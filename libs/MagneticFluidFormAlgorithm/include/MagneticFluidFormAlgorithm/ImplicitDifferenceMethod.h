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

    protected:
        ImplicitDifferenceMethod();

        virtual double calcUpperPhi(int i, double lowerGamma, double I1);

        virtual double calcLowerPhi(int i);

        virtual double calcLowerGamma(double I1, double I2);

        virtual double calcIntegral0();

        virtual double calcIntegral1();

        virtual double calcIntegral2();

    private:
        void calcIteration();

        void fillMatrices(
                std::vector<std::vector<double>> &radiusMatrix, std::vector<double> &radiusFreeMembers,
                std::vector<std::vector<double>> &heightMatrix, std::vector<double> &heightFreeMembers);
    };
}
