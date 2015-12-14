#pragma once

#include "../Core_global.h"

#include <QVariantMap>

namespace Core
{
    // Settings for generator. Each generator has it's own settings set and this settings set is hold by experiment.
    // NOTE: CGeneratorSettings MUST be QVariantMap. Typedef is used only for differ generator input, UI input and
    //       generator settings (CGeneratorInput, CGeneratorUIInput and CGeneratorSettings are more understandable as
    //       QVariantMap).
    // NOTE: Any value of generator settings must be one of the number type ([u]int32, [u]int64, double).
    //
    typedef QVariantMap CGeneratorSettings;
}