#include "QLogManip.h"
#include <QDebug>
#include <QRegExp>

//////////////////////////////////////////////////////////////////////////

const char *g_sManipToken      = "##";
const char *g_sModuleManipStr  = "MODULE:";
const char *g_sFuncManipStr    = "FUNCTION:";
const char *g_sSourceManipStr  = "SOURCE:";
const char *g_sManipContentStr = "(.*)";

const QString qlog_module::g_sRegExpStr = QString(g_sManipToken) +
    QString(g_sModuleManipStr) + QString(g_sManipContentStr) +
    QString(g_sManipToken);

const QString qlog_func::g_sRegExpStr = QString(g_sManipToken) +
    QString(g_sFuncManipStr) + QString(g_sManipContentStr) +
    QString(g_sManipToken);

const QString qlog_source::g_sRegExpStr = QString(g_sManipToken) +
    QString(g_sSourceManipStr) + QString(g_sManipContentStr) +
    QString(g_sManipToken);

//////////////////////////////////////////////////////////////////////////

qlog_module::qlog_module( const char *pModuleName /*= 0*/)
    : m_pModuleName(pModuleName) { }

bool qlog_module::isEmpty() const
{
    return (m_pModuleName == 0);
}

qlog_func::qlog_func( const char *pFuncName /*= 0*/)
    : m_pFuncName(pFuncName) { }

bool qlog_func::isEmpty() const
{
    return (m_pFuncName == 0);
}

qlog_source::qlog_source( const char *pFileName /*= 0*/,
    int iLineNumber /*= -1*/)
    : m_pFileName(pFileName), m_iLineNumber(iLineNumber) { }

bool qlog_source::isEmpty() const
{
    return ((m_pFileName == 0) || (m_iLineNumber == -1));
}

QDebug operator<<( QDebug dbg, const qlog_module &module )
{
    dbg.nospace() << "##MODULE:" << module.m_pModuleName << "##";
    return dbg.nospace();
}

QDebug operator<<( QDebug dbg, const qlog_func &fn )
{
    dbg.nospace() << "##FUNCTION:" << fn.m_pFuncName << "##";
    return dbg.nospace();
}

QDebug operator<<( QDebug dbg, const qlog_source &src )
{
    dbg.nospace() << "##SOURCE:" << src.m_pFileName << " - " <<
        src.m_iLineNumber << "##";
    return dbg.nospace();
}