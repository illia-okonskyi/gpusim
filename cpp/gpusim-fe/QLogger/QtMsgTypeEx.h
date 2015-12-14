#pragma once

#include <QtGlobal>

// WARNING: magic number here
//
namespace QtMsgTypeEx
{
    enum
    {
        QtInfoMsg = QtFatalMsg + 0x55,
        QtTraceMsg,
        QtTestMsg,
    };
}