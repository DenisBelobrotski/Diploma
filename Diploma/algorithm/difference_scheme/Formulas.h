#pragma once


#include "../Algorithm.h"


#define LOG_LOWER_PHI 0


namespace algorithm::ds
{
    double calcLowerPhi(int i, Variables *variables);

    double calcLowerGamma(Variables *variables, double I1, double I2);

    double calcUpperPhi(int i, Variables *variables, double lowerGamma, double I1);
}
