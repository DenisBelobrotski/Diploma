#include <MagneticFluidFormAlgorithm/MagneticFluidFormAlgorithm.h>


algorithm::ImplicitMethodUniformConcentration::ImplicitMethodUniformConcentration() : ImplicitDifferenceMethod()
{
}


algorithm::ImplicitMethodUniformConcentration::ImplicitMethodUniformConcentration(
        std::vector<Variables> *experimentVariables,
        std::vector<IterationInfo> *iterationsInfo)
        :
        ImplicitDifferenceMethod(experimentVariables, iterationsInfo)
{
}


algorithm::ImplicitMethodUniformConcentration::~ImplicitMethodUniformConcentration() = default;


#pragma MARK - Formulas

double algorithm::ImplicitMethodUniformConcentration::calcLowerGamma(double I1, double I2)
{
    double sum = variables.r[0] * cos(INITIAL_ALPHA) - variables.r[N] * sin(INITIAL_ALPHA) -
                 variables.B0 * variables.U / (2 * M_PI * variables.L) +
                 variables.A1 * I2;

    return 2 * sum / (variables.r[N] * variables.r[N] -
                      variables.r[0] * variables.r[0]);
}


double algorithm::ImplicitMethodUniformConcentration::calcUpperPhi(int i, double lowerGamma, double I1)
{
    double sum = -variables.A1 * calcLowerPhi(i) + lowerGamma;

    if (i == 0)
    {
        return variables.B0 * variables.L * variables.L * variables.z[i] + sum +
               variables.L * cos(INITIAL_ALPHA);
    }

    if (i == N)
    {
        return sum + sin(INITIAL_ALPHA) / variables.r[i];
    }

    return variables.B0 * variables.L * variables.L * variables.z[i] + sum -
           ((variables.z[i + 1] - variables.z[i - 1]) / (2 * STEP * variables.r[i]));
}
