#pragma once

#include "QLogger_global.h"
#include <QString>
#include <QDir>

// NOTE: when you finish working with logger or when you want to disable Qt
//       debug messages handling with this logger call QLogger::deinstall()
//       function.
//
// NOTE: If you want to use "file" logging destination you must set the
//       appropriate properties and then call buildLogFileFullPath() method.
//
class QLOGGER_EXPORT QLogger
{
public:
    typedef enum _tagCLogDestination
    {
        LogDest_NoOutput      = 0,
        LogDest_StdErr        = 0x00000001,
        LogDest_StdOut        = 0x00000002,
        LogDest_DebugOut      = 0x00000004,
        LogDest_File          = 0x00000008,
        LogDest_Default       = LogDest_StdErr | LogDest_DebugOut,
        LogDest_FileAndDbgOut = LogDest_File | LogDest_DebugOut,
        LogDest_All           = LogDest_Default | LogDest_StdOut | LogDest_File
    } CLogDestination;

    typedef enum _tagCLogLevel
    {
        LogLevel_OnlyFatalMsg    = 0x00000001,
        LogLevel_OnlyCriticalMsg = 0x00000002,
        LogLevel_OnlyWarningMsg  = 0x00000004,
        LogLevel_OnlyInfoMsg     = 0x00000008,
        LogLevel_OnlyDebugMsg    = 0x00000010,
        LogLevel_OnlyTraceMsg    = 0x00000020,
        LogLevel_OnlyTestMsg     = 0x00000040,
        
        LogLevel_Off     = 0,
        LogLevel_Fatal   = LogLevel_OnlyFatalMsg,
        LogLevel_Error   = LogLevel_Fatal | LogLevel_OnlyCriticalMsg,
        LogLevel_Warning = LogLevel_Error | LogLevel_OnlyWarningMsg,
        LogLevel_Info    = LogLevel_Warning | LogLevel_OnlyInfoMsg,
        LogLevel_Debug   = LogLevel_Info | LogLevel_OnlyDebugMsg,
        LogLevel_Trace   = LogLevel_Debug | LogLevel_OnlyTraceMsg,
        LogLevel_Test    = LogLevel_Trace | LogLevel_OnlyTestMsg,

        LogLevel_Default = LogLevel_Error,
    } CLogLevel;

public:
    // Get logger instance through this function. When you call it first it
    // creates an logger instance and installs Qt debug messages handling with
    // this logger.
    //
    static QLogger *instance();

    // Disables Qt debug messages handling and deletes the instance of the
    // logger.
    //
    static void deinstall();

    void setLogDestination(CLogDestination logDest = LogDest_Default);
    void setLogLevel(CLogLevel logLevel = LogLevel_Default);
    void setLogLevelAndLogIt(CLogLevel logLevel = LogLevel_Default);

    // Log format is the string from which log message is contructed.
    // If you pass an empty string to this method the default log format will
    // be used. Log format string can include texts and columns' tokens.
    // Column token has such format:
    //   %COL(ColumnName):ColumnSize%
    // Examples:
    //   %COL(SEV):4%; %COL(PID):4%; %COL(MSG):0%
    // This version of QLogger support the next columns (names of the columns
    // are given in brackets):
    //   - Severity (SEV);
    //   - Date-time (TIME);
    //   - PID (PID);
    //   - TID (TID);
    //   - Module name (MOD);
    //   - Function name (FUNC);
    //   - Source code position, eg. file and line (SRC);
    //   - Directly log message (MSG).
    //
    // Default log format is: [%COL(SEV):5%]:[%COL(TIME):25%]:%COL(MSG):0%
    //
    void setLogFormat(const QString &sLogFormat = QString());

    // DateTime stamp for message will converted by this format-string. To set
    // format-string to default pass an empty string to this method.
    // The default format-string is "yyyy.MM.dd - hh:mm:ss.zzz".
    //
    void setDateTimeFormat(const QString &sDateTimeFormat = QString());

    // Writes raw string into all log destinations. No formatting and columns
    // addition is applied to raw string.
    // 
    void writeRawStringToLog(const QString &sRawString);

    void writeEndl(int iCount = 1);
    void writeStartMessage();
    void writeStopMessage();
    void writeCurrentLogLevel();

    // Application name is used when creating default logger file's name.
    // In case of file log destination not set application name doesn't affect
    // something. To disable application name pass an empty string.
    // 
    void setAppName(const QString &sAppName = QString());

    // DateTime stamp is used when creating default logger file's name.
    // Current DateTime is converting to string by specified format-string.
    // To reset format-string to default pass an empty string.
    // The default-format string is "yyyy.MM.dd - hh.mm.ss".
    //
    void setLogFileDateTimeFormat(
        const QString &sLogFileDateTimeFormat = QString());

    // You can specify own log file name (with extenstion) instead of
    // generating default. To cancel custom log file name pass an empty string
    // to this method.
    // By default log file name is: "[<AppName>_]<CurrentDateTime>.log" (
    // value in brackets is optional and avaliable only if Application name is
    // set).
    // 
    void setLogFileName(const QString &sLogFileName = QString());

    // Log files will be created in this directory or opened from this
    // directory. By default logs directory is setted to temp directory.
    // 
    void setLogsDir(const QDir &logsDir);

    // When you have setted all "file" log destinations properties call this
    // method to build full path of the log file. When new message is accepted
    // logger opens file fith builded path (or creates new file if it doesn't
    // exists) and writes messages into this file.
    void buildLogFileFullPath();

    // Increases or decreases ident. Ident is two space characters.
    //
    void increaseIdent(size_t nCount = 1);
    void decreaseIdent(size_t nCount = 1);

private:
    QLogger();

    static void msgHandler(QtMsgType type, const char *pMsg);
    friend void installLoggerMsgHandler();

private:
    static QLogger *g_pInstance;

    CLogDestination m_logDest;
    QString m_sLogFormat;
    CLogLevel m_logLevel;
    QString m_sDateTimeFormat;

    QString m_sAppName;
    QString m_sLogFileDateTimeFormat;
    QString m_sUserLogFileName;
    QDir m_logsDir;

    QString m_sLogFileFullPath;

    size_t m_nIdent;
};