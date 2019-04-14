#pragma once


#include "DifferenceMethod.h"


#define LOG_LOWER_PHI 0


namespace algorithm
{
    int sgn(double value);

    double calcL(double I0, double U);

    void convertLengthToRadiusDimensionedVariables(Variables &variables);
}
