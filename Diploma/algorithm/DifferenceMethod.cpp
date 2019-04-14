#include "DifferenceMethod.h"
#include "Exceptions.h"
#include "../utils/Utils.h"
#include "Formulas.h"
#include <iostream>
#include <string>


algorithm::DifferenceMethod::DifferenceMethod() = default;


algorithm::DifferenceMethod::DifferenceMethod(
        std::vector<Variables> *experimentVariables,
        std::vector<IterationInfo> *iterationsInfo)
{
    if (experimentVariables == nullptr)
    {
        throw std::runtime_error("DifferenceMethod: experimentVariables is null");
    }
    if (iterationsInfo == nullptr)
    {
        throw std::runtime_error("DifferenceMethod: iterationsInfo is null");
    }

    this->experimentVariables = experimentVariables;
    this->iterationsInfo = iterationsInfo;

    variables.s = std::vector<double>(N + 1);
    variables.r = std::vector<double>(N + 1);
    variables.z = std::vector<double>(N + 1);
    variables.beta = std::vector<double>(N + 1);

    isNeedResetTau = false;
}


algorithm::DifferenceMethod::~DifferenceMethod() = default;


#pragma MARK - Algorithm

void algorithm::DifferenceMethod::fillVariables()
{
    double a = variables.s[0];
    for (auto i = 0; i < N + 1; i++)
    {
        variables.s[i] = LOWER_BOUND + i * STEP;

        variables.r[i] = 1 + variables.s[i];

        variables.z[N - i] = variables.s[i];

        variables.beta[i] = -M_PI_4;
    }

    variables.L = 0;
    variables.TAU = INITIAL_TAU;
    variables.U = INITIAL_U;
    variables.B0 = INITIAL_B0;
    variables.A1 = INITIAL_A1;
    variables.A2 = INITIAL_A2;
    variables.ALPHA = INITIAL_ALPHA;
}


void algorithm::DifferenceMethod::resetFields()
{
    iterationsCounter = 0;
    experimentsCounter = 0;
    isLastExperiment = false;
    fillVariables();
}


void algorithm::DifferenceMethod::changeParameter(
        double &parameter, double target, double step, long long drawRate, std::string parameterName) noexcept(false)
{
    if (isNeedResetTau)
    {
        variables.TAU = INITIAL_TAU;
    }

    bool isCalculated = false;
    const Variables startVariables = variables;

    if (drawRate != -2)
    {
        std::cout << "Start changing parameter " << parameterName << std::endl;
        std::cout << "Current value: " << parameter << ", target value: " << target << std::endl;
        std::cout << "Step: " << step << std::endl;
        std::cout << "////////////" << std::endl;
    }

    while (variables.TAU >= MIN_RELAXATION_PARAMETER && !isCalculated)
    {
        try
        {
            isLastExperiment = false;
            if (target > parameter)
            {
                increaseParameter(parameter, target, step, drawRate);
            }
            else if (target < parameter)
            {
                decreaseParameter(parameter, target, step, drawRate);
            }
            isCalculated = true;

        }
        catch (InvalidResultException &e)
        {
            std::cerr << "Invalid result:" << std::endl;
            std::cout << "relaxation parameter: " << std::endl;
            std::cout << "current: " << variables.TAU << ", ";
            variables.TAU *= RELAXATION_MULTIPLIER;
            std::cout << "new: " << variables.TAU << std::endl;
            std::cout << "-----" << std::endl;
        }
        catch (IterationsLimitException &e)
        {
            std::cerr << "Too many iterations:" << std::endl;
            std::cout << "relaxation parameter: " << std::endl;
            std::cout << "current: " << variables.TAU << ", ";
            variables.TAU *= RELAXATION_MULTIPLIER;
            std::cout << "new: " << variables.TAU << std::endl;
            std::cout << "-----" << std::endl;
        }

        if (!isCalculated)
        {
            double newTau = variables.TAU;
            variables = startVariables;
            variables.TAU = newTau;
        }
    }
    if (!isCalculated)
    {
        throw ParameterNotReachTargetValue(parameterName, target);
    }

    if (drawRate != -2)
    {
        std::cout << "Finish changing parameter " << parameterName << " to target value " << target << std::endl;
        std::cout << "////////////" << std::endl << std::endl;
    }
}

void algorithm::DifferenceMethod::increaseParameter(double &parameter, double target, double step, long long drawRate)
{
    while (parameter <= target)
    {
        if (parameter + step >= target)
        {
            isLastExperiment = true;
        }

        runExperiment(drawRate);

        if (isLastExperiment)
        {
            break;
        }

        parameter += step;
    }
}

void algorithm::DifferenceMethod::decreaseParameter(double &parameter, double target, double step, long long drawRate)
{
    while (parameter >= target)
    {
        if (parameter - step <= target)
        {
            isLastExperiment = true;
        }

        runExperiment(drawRate);

        if (isLastExperiment)
        {
            break;
        }

        parameter -= step;
    }
}


void algorithm::DifferenceMethod::runExperiment(long long drawRate)
{
    runIterationProcess();
    utils::clearProgressBar();

    pushExperimentResults(drawRate);

    experimentsCounter++;
}


void algorithm::DifferenceMethod::pushExperimentResults(long long drawRate)
{
    IterationInfo currentIterationInfo{};
    Variables resultVariables = variables;

    convertLengthToRadiusDimensionedVariables(resultVariables);

    experimentVariables->push_back(resultVariables);

    if (
            (drawRate > 0 && experimentsCounter % drawRate == 0)
            || (isLastExperiment && drawRate != -2)
            )
    {
        currentIterationInfo.index = experimentsCounter;
        currentIterationInfo.tau = variables.TAU;
        currentIterationInfo.u = variables.U;
        currentIterationInfo.b0 = variables.B0;
        currentIterationInfo.a1 = variables.A1;
        currentIterationInfo.a2 = variables.A2;
        currentIterationInfo.alpha = variables.ALPHA;

        iterationsInfo->push_back(currentIterationInfo);
    }
}


void algorithm::DifferenceMethod::calcResult()
{
    resetFields();

    iterationsCounter = 0;
    experimentsCounter = 0;

    try
    {
// ***************U*****************

        changeParameter(variables.U, 101, 1, -2, "U");

// ***************A2*****************

        changeParameter(variables.A2, 1, 0.05, -2, "A2");

        changeParameter(variables.A2, 3.0, 0.05, -1, "A2");

// ***************A1*****************

        changeParameter(variables.A1, 3.0, 0.1, -1, "A1");

//		changeParameter(variables.A1, 1.5, 0.1, -1, "A1");

        changeParameter(variables.A1, 1.0, 0.1, -1, "A1");

//		changeParameter(variables.A1, 0.5, 0.1, -1, "A1");

        changeParameter(variables.A1, 0.0, 0.1, -1, "A1");
    }
    catch (ParameterNotReachTargetValue &e)
    {
        std::cerr << "Parameter " << e.getParameterName() << " didn't reach target value "
                  << e.getTargetValue() << std::endl;
    }

    std::cout << "iterations count: " << iterationsCounter << std::endl;
    std::cout << "experiments count: " << experimentsCounter << std::endl;
    std::cout << "************************" << std::endl;
}


#pragma MARK - Setters/Getters

void algorithm::DifferenceMethod::setIsNeedResetTau(bool isNeedResetTau)
{
    this->isNeedResetTau = isNeedResetTau;
}


bool algorithm::DifferenceMethod::getIsNeedResetTau()
{
    return isNeedResetTau;
}
