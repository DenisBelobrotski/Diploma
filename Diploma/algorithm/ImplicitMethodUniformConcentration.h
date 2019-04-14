#pragma once

#include "ImplicitDifferenceMethod.h"


namespace algorithm
{
    class ImplicitMethodUniformConcentration : public ImplicitDifferenceMethod
    {
    public:
        ImplicitMethodUniformConcentration(
                std::vector<Variables> *experimentVariables, std::vector<IterationInfo> *iterationsInfo);

        ~ImplicitMethodUniformConcentration();

    protected:
        ImplicitMethodUniformConcentration();

        double calcLowerGamma(double I1, double I2) override;

        double calcUpperPhi(int i, double lowerGamma, double I1) override;
    };
}
