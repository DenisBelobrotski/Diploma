#pragma once

#include "ExplicitDifferenceMethod.h"


namespace algorithm
{
    class ExplicitMethodUniformConcentration : public ExplicitDifferenceMethod
    {
    public:
        ExplicitMethodUniformConcentration(
                std::vector<Variables> *experimentVariables, std::vector<IterationInfo> *iterationsInfo);

        ~ExplicitMethodUniformConcentration();

    protected:
        ExplicitMethodUniformConcentration();

        double calcLowerGamma(
                double r0, double r1, double I1, double I2, double L, double U, double B0, double A1,
                double ALPHA) override;

        double calcUpperPhi(
                double lowerGamma, double I1, double I2, double beta, double r, double z, double L, double U,
                double B0, double A1, double A2) override;
    };
}
