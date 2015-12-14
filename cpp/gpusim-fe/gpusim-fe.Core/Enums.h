#pragma once

#include "Core_global.h"

#include <QString>

namespace Core
{
    typedef enum _tagCExitCode
    {
        EC_None,
        EC_Ok,
        EC_Error,
        EC_Canceled

    } CExitCode;

    QString GPUSIM_FE_CORE_EXPORT exitCodeToString(CExitCode ec);
}