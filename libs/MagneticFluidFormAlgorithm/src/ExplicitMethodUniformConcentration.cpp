#include <MagneticFluidFormAlgorithm/MagneticFluidFormAlgorithm.h>


algorithm::ExplicitMethodUniformConcentration::ExplicitMethodUniformConcentration() : ExplicitDifferenceMethod()
{
}


algorithm::ExplicitMethodUniformConcentration::ExplicitMethodUniformConcentration(
        std::vector<Variables> *experimentVariables,
        std::vector<IterationInfo> *iterationsInfo)
        :
        ExplicitDifferenceMethod(experimentVariables, iterationsInfo)
{
}


algorithm::ExplicitMethodUniformConcentration::~ExplicitMethodUniformConcentration() = default;


#pragma MARK - Formulas

double algorithm::ExplicitMethodUniformConcentration::calcLowerGamma(
        double r0, double r1, double I1, double I2, double L, double U, double B0, double A1, double ALPHA)
{
    return (2 / (r1 * r1 - r0 * r0)) * (r0 * cos(ALPHA) - r1 * sin(ALPHA) - (B0 * U) / (2 * M_PI * L) +
                                        A1 * I2);
}


double algorithm::ExplicitMethodUniformConcentration::calcUpperPhi(
        double lowerGamma, double I1, double I2, double beta, double r, double z, double L, double U, double B0,
        double A1, double A2)
{
    return B0 * z * L * L - A1 * calcLowerPhi(r, L, A2) - sin(beta) / r + lowerGamma;
}