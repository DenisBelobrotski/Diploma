#pragma once


#include <MagneticFluidFormAlgorithm/MagneticFluidFormAlgorithm.h>


namespace algorithm
{
    int sgn(double value);

    double calcL(double I0, double U);

    void convertLengthToRadiusDimensionedVariables(Variables &variables);
}
