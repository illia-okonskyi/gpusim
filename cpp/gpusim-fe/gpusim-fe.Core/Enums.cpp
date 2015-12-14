#include "Enums.h"

QString Core::exitCodeToString(CExitCode ec)
{
    switch(ec)
    {
    case EC_None:
        return QString("EC_None");

    case EC_Ok:
        return QString("EC_Ok");

    case EC_Error:
        return QString("EC_Error");

    case EC_Canceled:
        return QString("EC_Canceled");

    default:
        return QString("<Wrong exit code value>");
    }
}