#pragma once

#include <QtCore/qglobal.h>

#ifdef GPUSIM_FE_TOOLS_LIB
#   define GPUSIM_FE_TOOLS_EXPORT Q_DECL_EXPORT
#else
#   define GPUSIM_FE_TOOLS_EXPORT Q_DECL_IMPORT
#   ifdef _DEBUG
#       pragma comment(lib, "gpusim-fe.Toolsd.lib")
#   else
#       pragma comment(lib, "gpusim-fe.Tools.lib")
#   endif // _DEBUG
#endif // GPUSIM_FE_TOOLS_LIB