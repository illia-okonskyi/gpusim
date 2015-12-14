#include "QLogger.h"
#include "QtMsgTypeEx.h"
#include "QLogManip.h"

#include <QDateTime>
#include <QFile>
#include <Windows.h>
#include <QTextStream>
#include <QStringList>
#include <tuple>
#include <vector>
#include <cassert>

QLogger *QLogger::g_pInstance = 0;
const QString g_sDefaultLogFileDateTimeFormat = "yyyy.MM.dd - hh.mm.ss";
const QString g_sDefaultDateTimeFormat = "yyyy.MM.dd - hh:mm:ss.zzz";
const QString g_sDefaultLogFormat = "[%COL(SEV):5%]:[%COL(TIME):25%]:"
    "%COL(MSG):0%";

const QString g_psColumnNames [] = {"SEV", "TIME", "PID", "TID",
    "MOD", "FUNC", "SRC", "MSG"};
const size_t g_nColumnsCount =
    sizeof(g_psColumnNames) / sizeof(g_psColumnNames[0]);

//////////////////////////////////////////////////////////////////////////
template <class qlog_manip>
QString parseManip(QString &sMsg)
{
    QString sManipContent;
    QRegExp rx(qlog_manip::g_sRegExpStr);
    rx.setMinimal(true);

    if (rx.indexIn(sMsg) != -1)
    {
        sManipContent = rx.cap(1);
        sMsg.replace(rx, QString());
    }
    return sManipContent;
}

//////////////////////////////////////////////////////////////////////////

inline void installLoggerMsgHandler()
{
    qInstallMsgHandler(QLogger::msgHandler);
}

inline void deinstallLoggerMsgHandler()
{
    qInstallMsgHandler(0);
}

bool isMessageAcceptableForLogLevel( QLogger::CLogLevel logLevel,
    QtMsgType type )
{
    QLogger::CLogLevel logLevelForMessage = QLogger::LogLevel_Off;
    switch (type)
    {
    case QtFatalMsg:
        logLevelForMessage = QLogger::LogLevel_OnlyFatalMsg;
        break;
    case QtCriticalMsg:
        logLevelForMessage = QLogger::LogLevel_OnlyCriticalMsg;
        break;
    case QtWarningMsg:
        logLevelForMessage = QLogger::LogLevel_OnlyWarningMsg;
        break;
    case QtMsgTypeEx::QtInfoMsg:
        logLevelForMessage = QLogger::LogLevel_OnlyInfoMsg;
        break;
    case QtDebugMsg:
        logLevelForMessage = QLogger::LogLevel_OnlyDebugMsg;
        break;
    case QtMsgTypeEx::QtTraceMsg:
        logLevelForMessage = QLogger::LogLevel_OnlyTraceMsg;
        break;
    case QtMsgTypeEx::QtTestMsg:
        logLevelForMessage = QLogger::LogLevel_OnlyTestMsg;
        break;
    }

    return (logLevel & logLevelForMessage);
}

QString msgTypeString(QtMsgType type)
{
    QString sType;
    switch (type)
    {
    case QtFatalMsg:
        sType = "FATAL";
        break;
    case QtCriticalMsg:
        sType = "CRIT";
        break;
    case QtWarningMsg:
        sType = "WARN";
        break;
    case QtMsgTypeEx::QtInfoMsg:
        sType = "INFO";
        break;
    case QtDebugMsg:
        sType = "DEBUG";
        break;
    case QtMsgTypeEx::QtTraceMsg:
        sType = "TRACE";
        break;
    case QtMsgTypeEx::QtTestMsg:
        sType = "TEST";
        break;

    default:
        sType = "UNKNW";
        break;
    }

    return sType;
}

QString logLevelString(QLogger::CLogLevel ll)
{
    QString sLL;
    switch (ll)
    {
    case QLogger::LogLevel_OnlyCriticalMsg:
        sLL = "OnlyCriticalMsg";
        break;
    case QLogger::LogLevel_OnlyWarningMsg:
        sLL = "OnlyWarningMsg";
        break;
    case QLogger::LogLevel_OnlyInfoMsg:
        sLL = "OnlyInfoMsg";
        break;
    case QLogger::LogLevel_OnlyDebugMsg:
        sLL = "OnlyDebugMsg";
        break;
    case QLogger::LogLevel_OnlyTraceMsg:
        sLL = "OnlyTraceMsg";
        break;
    case QLogger::LogLevel_OnlyTestMsg:
        sLL = "OnlyTestMsg";
        break;

    case QLogger::LogLevel_Off:
        sLL = "Off";
        break;
    case QLogger::LogLevel_Fatal:
        sLL = "Fatal";
        break;
    case QLogger::LogLevel_Error:
        sLL = "Error";
        break;
    case QLogger::LogLevel_Warning:
        sLL = "Warning";
        break;
    case QLogger::LogLevel_Info:
        sLL = "Info";
        break;
    case QLogger::LogLevel_Debug:
        sLL = "Debug";
        break;
    case QLogger::LogLevel_Trace:
        sLL = "Trace";
        break;
    case QLogger::LogLevel_Test:
        sLL = "Test";
        break;

    default:
        sLL = "Wrong";
        break;
    }

    return sLL;
}

QString currentDateTimeString(const QString &sUserFormat = QString())
{
    QString sFormat = sUserFormat;
    if (sFormat.isEmpty())
        sFormat = g_sDefaultDateTimeFormat;

    return QDateTime::currentDateTime().toString(sFormat);
}

// NOTE: If column name isn't recognized - column value is the column name.
//
QString mapColumnNameToValue(const QString &sColName,
    const std::vector<QString> &columnValues)
{
    assert(columnValues.size() == g_nColumnsCount);

    for (size_t i = 0; i < g_nColumnsCount; ++i)
    {
        if (sColName == g_psColumnNames[i])
            return columnValues[i];
    }

    return sColName;
}

QString buildMessage(const QString &sLogFormat,  const QString &sDateTimeFormat,
    QtMsgType type, const char *pMsg, size_t nIdent)
{
    // CColumn type (tuple)
    //   <0> - column value (text);
    //   <1> - column size.
    typedef std::tr1::tuple<QString, int> CColumn;

    QString sRawMsg = QString::fromLocal8Bit(pMsg);

    // 1) Get all columns values in such sequence:
    //   - Severity;
    //   - DateTime;
    //   - PID;
    //   - TID;
    //   - Module name;
    //   - Function name;
    //   - Source;
    //   - Message.
    //
    std::vector<QString> columnValues;
    columnValues.push_back(msgTypeString(type));
    columnValues.push_back(currentDateTimeString(sDateTimeFormat));
    columnValues.push_back(QString::number(GetCurrentProcessId()));
    columnValues.push_back(QString::number(GetCurrentThreadId()));
    columnValues.push_back(parseManip<qlog_module>(sRawMsg));
    columnValues.push_back(parseManip<qlog_func>(sRawMsg));
    columnValues.push_back(parseManip<qlog_source>(sRawMsg));
    columnValues.push_back(sRawMsg);

    //
    // 2) Parse format string for spliting it to the columns and texts.
    //
    QRegExp rxColumn("%COL\\((.+)\\):(\\d+)%");
    rxColumn.setMinimal(true);

    QStringList formatTexts = sLogFormat.split(rxColumn);
    std::vector<CColumn> columns;

    if (formatTexts.size() > 1)
    {
        int iLastIndex = 0;
        while((iLastIndex = rxColumn.indexIn(sLogFormat, iLastIndex)) != -1)
        {
            columns.push_back(
                CColumn(mapColumnNameToValue(rxColumn.cap(1), columnValues),
                rxColumn.cap(2).toInt()));

            iLastIndex += rxColumn.matchedLength();
        }
    }

    assert(columns.size() == formatTexts.size() - 1);

    // 3) Build log message
    //
    QString sLogMessage;
    QTextStream tsLogMessage(&sLogMessage);
    tsLogMessage << formatTexts[0];
    for(size_t i = 0; i < columns.size(); ++i)
    {
        QString sColValue = std::tr1::get<0>(columns[i]);
        int iColFieldWidth = std::tr1::get<1>(columns[i]);

        // Truncate very long column value.
        //
        if ((iColFieldWidth > 0) && (sColValue.size() > iColFieldWidth))
        {
            sColValue.truncate(iColFieldWidth);
            sColValue.replace(iColFieldWidth - 3, 3, "...");
        }

        tsLogMessage.setFieldAlignment(QTextStream::AlignCenter);
        tsLogMessage.setFieldWidth(iColFieldWidth);
        tsLogMessage << sColValue;
        tsLogMessage.setFieldWidth(0);
        tsLogMessage << formatTexts[i + 1];
    }
    tsLogMessage << endl;
    return sLogMessage;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

QLogger::QLogger()
    : m_logDest(LogDest_Default),  m_sLogFormat(g_sDefaultLogFormat),
    m_logLevel(LogLevel_Default), m_logsDir(QDir::temp()), m_nIdent(0)
{

}

QLogger * QLogger::instance()
{
    if (!g_pInstance)
    {
        g_pInstance = new QLogger();
        installLoggerMsgHandler();
    }

    return g_pInstance;
}

void QLogger::deinstall()
{
    if (!g_pInstance)
        return;

    deinstallLoggerMsgHandler();
    delete g_pInstance;
    g_pInstance = 0;
}

//////////////////////////////////////////////////////////////////////////

void QLogger::setLogDestination( CLogDestination logDest /*= LogDest_Default*/)
{
    m_logDest = logDest;
}

void QLogger::setLogLevel( CLogLevel logLevel /*= LogLevel_Default*/ )
{
    m_logLevel = logLevel;
}

void QLogger::setLogLevelAndLogIt( CLogLevel logLevel /*= LogLevel_Default*/ )
{
    const QString sFiller(20, '*');
    QString sFormat("%1 Log level changed from <%2> to <%3>.\n");
    writeRawStringToLog(sFormat.arg(sFiller, logLevelString(m_logLevel),
        logLevelString(logLevel)));

    setLogLevel(logLevel);
}


void QLogger::setLogFormat( const QString &sLogFormat /*= QString()*/ )
{
    m_sLogFormat = sLogFormat.isEmpty()? g_sDefaultLogFormat :  sLogFormat;
}

void QLogger::setDateTimeFormat( const QString &sDateTimeFormat /*= QString()*/)
{
    m_sDateTimeFormat = sDateTimeFormat;
}

void QLogger::setAppName( const QString &sAppName /*= QString()*/ )
{
    m_sAppName = sAppName;
}

//////////////////////////////////////////////////////////////////////////

void QLogger::writeRawStringToLog( const QString &sRawString )
{
    //
    // 1) Chose output destinations.
    //

    std::vector<QFile *> destinations;

    if (m_logDest & LogDest_StdErr)
    {
        QFile *pFile = new QFile();
        pFile->open(stderr, QIODevice::WriteOnly);
        destinations.push_back(pFile);
    }

    if (m_logDest & LogDest_StdOut)
    {
        QFile *pFile = new QFile();
        pFile->open(stdout, QIODevice::WriteOnly);
        destinations.push_back(pFile);
    }

    if (m_logDest & LogDest_File)
    {
        if (m_sLogFileFullPath.isEmpty())
            buildLogFileFullPath();

        QFile *pFile = new QFile(m_sLogFileFullPath);
        pFile->open(QIODevice::WriteOnly | QIODevice::Append);
        destinations.push_back(pFile);
    }

    // 2) Write output message to all file destinations.
    //
    foreach(QFile *pDestFile, destinations)
    {
        pDestFile->write(sRawString.toUtf8());
        pDestFile->flush();
        pDestFile->close();
        pDestFile->deleteLater();
    }

    // 3) Write output message to the debug output.
    //
    if (m_logDest & LogDest_DebugOut)
        OutputDebugStringW(reinterpret_cast<LPCWSTR>(sRawString.utf16()));
}

void QLogger::writeStartMessage()
{
    const QString sSmallFiller(19, '*');
    const QString sLongFiller(80, '*');
    QString sFormat("%1\n%2 Log started at %3 %2\n%1\n");
    writeRawStringToLog(
        sFormat.arg(sLongFiller, sSmallFiller, currentDateTimeString()));
}

void QLogger::writeStopMessage()
{
    const QString sSmallFiller(19, '*');
    const QString sLongFiller(80, '*');
    QString sFormat("%1\n%2 Log stopped at %3 %2\n%1\n");
    writeRawStringToLog(
        sFormat.arg(sLongFiller, sSmallFiller, currentDateTimeString()));
}

void QLogger::writeCurrentLogLevel()
{
    const QString sFiller(20, '*');
    QString sFormat("%1 Current log level is <%2>.\n");
    writeRawStringToLog(sFormat.arg(sFiller, logLevelString(m_logLevel)));
}

void QLogger::writeEndl( int iCount /*= 1*/ )
{
    writeRawStringToLog(QString(iCount, '\n'));
}

//////////////////////////////////////////////////////////////////////////

void QLogger::setLogFileDateTimeFormat(
    const QString &sLogFileDateTimeFormat /*= QString()*/ )
{
    m_sLogFileDateTimeFormat = sLogFileDateTimeFormat;
}

void QLogger::setLogFileName( const QString &sLogFileName /*= QString()*/ )
{
    m_sUserLogFileName = sLogFileName;
}

void QLogger::setLogsDir( const QDir &logsDir )
{
    m_logsDir = logsDir;
}

void QLogger::buildLogFileFullPath()
{
    QString sRelativePath(m_sUserLogFileName);
    if (sRelativePath.isEmpty())
    {
        sRelativePath = m_sAppName;
        if (!sRelativePath.isEmpty())
            sRelativePath += " - ";

        sRelativePath += QDateTime::currentDateTime().toString(
            m_sLogFileDateTimeFormat.isEmpty()? g_sDefaultLogFileDateTimeFormat:
            m_sLogFileDateTimeFormat);

        sRelativePath += ".log";
    }

    m_sLogFileFullPath = QDir::toNativeSeparators(
        m_logsDir.absoluteFilePath(sRelativePath));
}

void QLogger::increaseIdent( size_t nCount /*= 1*/ )
{
    m_nIdent += nCount;
}

void QLogger::decreaseIdent( size_t nCount /*= 1*/ )
{
    if (nCount > m_nIdent)
        m_nIdent = 0;
    else
        m_nIdent -= nCount;
}

//////////////////////////////////////////////////////////////////////////

void QLogger::msgHandler( QtMsgType type, const char *pMsg )
{
    // 1) Write this message into log?.
    // 
    if (!isMessageAcceptableForLogLevel(g_pInstance->m_logLevel, type))
        return;

    // 2) Deinstall logger message handler to avoid stack overflows
    deinstallLoggerMsgHandler();

    // 3) Make the output message.
    QString sMessage = buildMessage(g_pInstance->m_sLogFormat,
        g_pInstance->m_sDateTimeFormat, type, pMsg, g_pInstance->m_nIdent);

    // 4) Write builded message to all logs destinations.
    g_pInstance->writeRawStringToLog(sMessage);

    // 5) Renew logger message handler.
    installLoggerMsgHandler();
}