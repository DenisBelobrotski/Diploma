#include "ExplicitDifferenceMethod.h"
#include "Formulas.h"
#include "../utils/Utils.h"
#include "Exceptions.h"


algorithm::ExplicitDifferenceMethod::ExplicitDifferenceMethod() : DifferenceMethod()
{
}


algorithm::ExplicitDifferenceMethod::ExplicitDifferenceMethod(
        std::vector<Variables> *experimentVariables,
        std::vector<IterationInfo> *iterationsInfo)
        :
        DifferenceMethod(experimentVariables, iterationsInfo)
{
}


algorithm::ExplicitDifferenceMethod::~ExplicitDifferenceMethod() = default;


#pragma MARK - Algorithm

void algorithm::ExplicitDifferenceMethod::calcBeta()
{
    double r0 = variables.r[0];
    double r1 = variables.r[N];

    double I0 = calcIntegral0();
    double L = variables.L = calcL(I0, variables.U);
    double I1 = calcIntegral1();
    double I2 = calcIntegral2();

    double lowerGamma = calcLowerGamma(r0, r1, I1, I2, L,
                                       variables.U, variables.B0,
                                       variables.A1, variables.ALPHA);

#if SIMPLE_RELAXATION_FORMULA
    std::vector<double> prevBeta = variables.beta;
#endif

    for (auto i = N - 1; i > 0; i--)
    {
        double tmpZ = (variables.z[i] + variables.z[i + 1]) / 2;
        double tmpR = (variables.r[i] + variables.r[i + 1]) / 2;
        double tmpBeta = (variables.beta[i] + variables.beta[i + 1]) / 2;

        double upperPhi = calcUpperPhi(lowerGamma, I1, I2, tmpBeta, tmpR, tmpZ, L,
                                       variables.U, variables.B0, variables.A1, variables.A2);

#if SIMPLE_RELAXATION_FORMULA
        variables.beta[i] = variables.beta[i + 1] - STEP * upperPhi;
#else
        variables.beta[i] = variables.beta[i + 1] - STEP * upperPhi +
            (1 - variables.TAU) * (variables.beta[i] - variables.beta[i + 1] + STEP * upperPhi);
#endif
    }

#if SIMPLE_RELAXATION_FORMULA
    for (auto i = 0; i < N + 1; i++)
    {
        variables.beta[i] = (1 - variables.TAU) * prevBeta[i] + variables.TAU * variables.beta[i];
    }
#endif
}


void algorithm::ExplicitDifferenceMethod::calcRadius()
{
    variables.r[0] = 1 / variables.L;

    for (auto i = 1; i < N + 1; i++)
    {
        double tmpBeta = (variables.beta[i - 1] + variables.beta[i]) / 2;
        variables.r[i] = variables.r[i - 1] + STEP * cos(tmpBeta);
    }
}


void algorithm::ExplicitDifferenceMethod::calcHeight()
{
    variables.z[N] = 0;

    for (auto i = N - 1; i >= 0; i--)
    {
        double tmpBeta = (variables.beta[i] + variables.beta[i + 1]) / 2;
        variables.z[i] = variables.z[i + 1] - STEP * sin(tmpBeta);
    }
}


void algorithm::ExplicitDifferenceMethod::calcIteration()
{
    calcBeta();
    calcRadius();
    calcHeight();
}


void algorithm::ExplicitDifferenceMethod::runIterationProcess() noexcept(false)
{
    long long startIterationsCounter = iterationsCounter;
    double residual;

    std::vector<double> prevR;
    std::vector<double> prevZ;

    do
    {
        prevR = variables.r;
        prevZ = variables.z;

        calcIteration();

        residual = std::max(utils::calcResidual(prevR, variables.r),
                            utils::calcResidual(prevZ, variables.z));

        iterationsCounter++;

        if (iterationsCounter - startIterationsCounter > MAX_ITERATIONS_NUMBER)
        {
            throw IterationsLimitException();
        }
        else
        {
            utils::showIterationsProgressBar(iterationsCounter - startIterationsCounter, MAX_ITERATIONS_NUMBER);
        }
    } while (residual > ACCURACY);

    if (!utils::isValid(variables.r) || !utils::isValid(variables.z))
    {
        utils::clearProgressBar();
        throw InvalidResultException();
    }
}

#pragma MARK - Formulas

double algorithm::ExplicitDifferenceMethod::calcLowerPhi(double r, double L, double A2)
{
    if (A2 == 0.0)
    {
        return 1;
    }
    double tmp = r * L / A2;

    double invertTmp = 1 / tmp;

    double sinH = sinh(invertTmp);

    double result = tmp * sinH;

    return result;
}


double algorithm::ExplicitDifferenceMethod::calcLowerGamma(
        double r0, double r1, double I1, double I2, double L, double U, double B0, double A1, double ALPHA)
{
    return (2 / (r1 * r1 - r0 * r0)) * (r0 * cos(ALPHA) - r1 * sin(ALPHA) - (B0 * U) / (2 * M_PI * L) +
                                        (A1 * U * I2) / (I1 * L * L));
}


double algorithm::ExplicitDifferenceMethod::calcUpperPhi(
        double lowerGamma, double I1, double I2, double beta, double r, double z, double L, double U, double B0,
        double A1, double A2)
{
    return B0 * z * L * L - (A1 * U * calcLowerPhi(r, L, A2)) / (I1 * L * L) - sin(beta) / r + lowerGamma;
}


#pragma MARK - Integrals

double algorithm::ExplicitDifferenceMethod::calcIntegral0()
{
    double result = 0;

    for (auto i = 1; i < N + 1; i++)
    {
        double tmpZ = (variables.z[i - 1] + variables.z[i]) / 2;
        double tmpR = (variables.r[i - 1] + variables.r[i]) / 2;
        double tmpBeta = (variables.beta[i - 1] + variables.beta[i]) / 2;
        double tmp = tmpZ * tmpR * cos(tmpBeta);
        result += tmp;
    }

    result *= 2 * M_PI * STEP;

    return result;
}


double algorithm::ExplicitDifferenceMethod::calcIntegral1()
{
    double result = 0;

    for (auto i = 1; i < N + 1; i++)
    {
        double tmpZ = (variables.z[i - 1] + variables.z[i]) / 2;
        double tmpR = (variables.r[i - 1] + variables.r[i]) / 2;
        double tmpBeta = (variables.beta[i - 1] + variables.beta[i]) / 2;
        double lowerPhi = calcLowerPhi(tmpR, variables.L, variables.A2);
        double tmp = lowerPhi * tmpZ * tmpR * cos(tmpBeta);
        result += tmp;
    }

    result *= 2 * M_PI * STEP;

    return result;
}


double algorithm::ExplicitDifferenceMethod::calcIntegral2()
{
    double result = 0;

    for (auto i = 1; i < N + 1; i++)
    {
        double tmpR = (variables.r[i - 1] + variables.r[i]) / 2;
        double tmpBeta = (variables.beta[i - 1] + variables.beta[i]) / 2;
        double lowerPhi = calcLowerPhi(tmpR, variables.L, variables.A2);
        double tmp = lowerPhi * tmpR * cos(tmpBeta);
        result += tmp;
    }

    result *= STEP;

    return result;
}
