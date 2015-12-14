#pragma once

#include <QtCore/qglobal.h>

#ifdef QLOGGER_LIB
#   define QLOGGER_EXPORT Q_DECL_EXPORT
#else
#   define QLOGGER_EXPORT Q_DECL_IMPORT
#   ifdef _DEBUG
#       pragma comment(lib, "QLoggerd.lib")
#   else
#       pragma comment(lib, "QLogger.lib")
#   endif // _DEBUG
#endif // QLOGGER_LIB

