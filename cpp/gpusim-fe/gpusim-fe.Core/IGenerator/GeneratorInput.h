#pragma once

#include "../Core_global.h"

#include <QVariantMap>
#include <QVector>
#include <QHash>

namespace Core
{
    // Input Parameters set for generator. Each generator has own input parameters which should be set by
    // experiment-plugin user.
    // NOTE: CGeneratorInput MUST be QVariantMap. Typedef is used only for differ generator input, UI input and
    //       generator settings (CGeneratorInput, CGeneratorUIInput and CGeneratorSettings are more understandable as
    //       QVariantMap).
    // NOTE: Any value of generator input must be one of the number type ([u]int32, [u]int64, double).
    // 
    typedef QVariantMap CGeneratorInput;
}

inline uint qHash(const Core::CGeneratorInput &input)
{
    QString s;
    for (auto I = input.constBegin(), E = input.constEnd(); I != E; ++I)
        s.append(QString("%1-%2").arg(I.key(), I->toString()));

    return qHash(s);
}