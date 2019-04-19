#pragma once

#include "DifferenceMethod.h"


#define SIMPLE_RELAXATION_FORMULA 1


namespace algorithm
{
    class ExplicitDifferenceMethod : public DifferenceMethod
    {
    public:
        ExplicitDifferenceMethod(
                std::vector<Variables> *experimentVariables,
                std::vector<IterationInfo> *iterationsInfo);

        ~ExplicitDifferenceMethod();

        void runIterationProcess() noexcept(false) override;

    protected:
        ExplicitDifferenceMethod();

        virtual double calcLowerPhi(double r, double L, double A2);

        virtual double calcLowerGamma(
                double r0, double r1, double I1, double I2, double L, double U, double B0, double A1, double ALPHA);

        virtual double calcUpperPhi(
                double lowerGamma, double I1, double I2, double beta, double r, double z, double L, double U,
                double B0, double A1, double A2);

        virtual double calcIntegral0();

        virtual double calcIntegral1();

        virtual double calcIntegral2();

    private:
        void calcBeta();

        void calcRadius();

        void calcHeight();

        void calcIteration();
    };
}
