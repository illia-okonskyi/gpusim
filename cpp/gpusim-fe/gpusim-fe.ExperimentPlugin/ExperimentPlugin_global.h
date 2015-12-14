#pragma once

#include <QtCore/qglobal.h>

#ifdef GPUSIM_FE_EXPERIMENTPLUGIN_LIB
#   define GPUSIM_FE_EXPERIMENTPLUGIN_EXPORT Q_DECL_EXPORT
#else
#   define GPUSIM_FE_EXPERIMENTPLUGIN_EXPORT Q_DECL_IMPORT
#   ifdef _DEBUG
#       pragma comment(lib, "gpusim-fe.ExperimentPlugind.lib")
#   else
#       pragma comment(lib, "gpusim-fe.ExperimentPlugin.lib")
#   endif // _DEBUG
#endif // GPUSIM_FE_EXPERIMENTPLUGIN_LIB