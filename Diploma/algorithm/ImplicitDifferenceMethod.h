#pragma once

#include "DifferenceMethod.h"


namespace algorithm
{
    class ImplicitDifferenceMethod : public DifferenceMethod
    {
    public:
        ImplicitDifferenceMethod(
                std::vector<Variables> *experimentVariables, std::vector<IterationInfo> *iterationsInfo);

        ~ImplicitDifferenceMethod();

        void runIterationProcess() noexcept(false) override;

    private:
        ImplicitDifferenceMethod();

        void calcIteration();

        void fillMatrices(
                std::vector<std::vector<double>> &radiusMatrix, std::vector<double> &radiusFreeMembers,
                std::vector<std::vector<double>> &heightMatrix, std::vector<double> &heightFreeMembers);

        double calcLowerPhi(int i);

        double calcLowerGamma(double I1, double I2);

        double calcUpperPhi(int i, double lowerGamma, double I1);

        double calcIntegral0();

        double calcIntegral1();

        double calcIntegral2();
    };
}
