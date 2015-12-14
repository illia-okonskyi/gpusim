#pragma once

#include "QLogger_global.h"

struct QLOGGER_EXPORT qlog_module
{
    qlog_module(const char *pModuleName = 0);
    bool isEmpty() const;

    const char *m_pModuleName;

    static const QString g_sRegExpStr;
};

struct QLOGGER_EXPORT qlog_func
{
    qlog_func(const char *pFuncName = 0);
    bool isEmpty() const;

    const char *m_pFuncName;

    static const QString g_sRegExpStr;
};

struct QLOGGER_EXPORT qlog_source
{
    qlog_source(const char *pFileName = 0, int iLineNumber = -1);
    bool isEmpty() const;

    const char *m_pFileName;
    int m_iLineNumber;

    static const QString g_sRegExpStr;
};

#ifdef QLOG_MODULE_NAME
#   define qlog_cur_module qlog_module(QLOG_MODULE_NAME)
#else
#   define qlog_cur_module qlog_module()
#endif // QLOG_MODULE_NAME

#define qlog_cur_func (qlog_func(__FUNCTION__))
#define qlog_cur_src (qlog_source(__FILE__, __LINE__))

QLOGGER_EXPORT QDebug operator<<(QDebug dbg, const qlog_module &module);
QLOGGER_EXPORT QDebug operator<<(QDebug dbg, const qlog_func &fn);
QLOGGER_EXPORT QDebug operator<<(QDebug dbg, const qlog_source &src);

