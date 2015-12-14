#pragma once

#include "../Core_global.h"

#include <QVariantMap>

namespace Core
{
    // Boundary conditions for experiment should be created by generator. Are edited by user and should be part of
    // experiment.
    // NOTE: CGeneratorBoundaryConditions MUST be QVariantMap. Typedef is used only for differ generator input, settings 
    //       and border conditions. (CGeneratorInput, CGeneratorSettings and CGeneratorBorderConditions are more
    //       understandable as QVariantMap).
    // NOTE: Any value of generator border condition must be one of the number type ([u]int32, [u]int64, double).
    // 
    typedef QVariantMap CGeneratorBoundaryConditions;
}