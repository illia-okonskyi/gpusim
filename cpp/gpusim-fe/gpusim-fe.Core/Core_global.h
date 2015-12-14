#pragma once

#include <QtCore/qglobal.h>

#ifdef GPUSIM_FE_CORE_LIB
#   define GPUSIM_FE_CORE_EXPORT Q_DECL_EXPORT
#else
#   define GPUSIM_FE_CORE_EXPORT Q_DECL_IMPORT
#   ifdef _DEBUG
#       pragma comment(lib, "gpusim-fe.Cored.lib")
#   else
#       pragma comment(lib, "gpusim-fe.Core.lib")
#   endif // _DEBUG
#endif // GPUSIM_FE_CORE_LIB