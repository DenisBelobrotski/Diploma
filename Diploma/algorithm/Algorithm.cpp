#include "Algorithm.h"
#include "Exceptions.h"
#include "../utils/Utils.h"
#include <iostream>
#include <string>


namespace algorithm
{
	void fillVariables(Variables *variables);

	void changeParameter(void(*runIterationProcess) (Variables &, long long &) noexcept(false),
						 Variables &variables, double &parameter, double target, double step,
                         long long &iterationsCounter, long long &experimentsCounter,
                         std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                         long long drawRate, std::string parameterName) noexcept(false);

	void increaseParameter(void(*runIterationProcess) (Variables &, long long &) noexcept(false),
						   Variables &variables, double &parameter, double target, double step,
                           long long &iterationsCounter, long long &experimentsCounter,
                           std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                           long long drawRate) noexcept(false);

	void decreaseParameter(void(*runIterationProcess) (Variables &, long long &) noexcept(false),
						   Variables &variables, double &parameter, double target, double step,
                           long long &iterationsCounter, long long &experimentsCounter,
                           std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                           long long drawRate) noexcept(false);

    void runExperiment(void(*runIterationProcess) (Variables &, long long &) noexcept(false),
					   Variables &variables, long long &iterationsCounter, long long &experimentsCounter,
                       std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                       bool isLastExperiment, long long drawRate) noexcept(false);

	void pushExperimentResults(long long &experimentsCounter, Variables &variables,
	                           std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                               bool isLastExperiment, long long drawRate);

    void convertToRadiusDimensionedVariables(Variables &variables);
}


void algorithm::fillVariables(algorithm::Variables *variables)
{
	for (int i = 0; i < N + 1; i++)
	{
		variables->s[i] = LOWER_BOUND + i * STEP;

		variables->r[i] = variables->s[i];

		variables->z[N - i] = variables->s[i];

		variables->beta[i] = -M_PI_4;
	}

	variables->L = 0;
	variables->TAU = INITIAL_TAU;
	variables->U = INITIAL_U;
	variables->B0 = INITIAL_B0;
	variables->A1 = INITIAL_A1;
	variables->A2 = INITIAL_A2;
	variables->ALPHA = INITIAL_ALPHA;
}


void algorithm::changeParameter(void (*runIterationProcess) (Variables &, long long &) noexcept(false),
								Variables &variables, double &parameter, double target, double step,
                                long long &iterationsCounter, long long &experimentsCounter,
                                std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                                long long drawRate, const std::string parameterName) noexcept(false)
{
#if ALWAYS_RESET_TAU
	variables.TAU = INITIAL_TAU;
#endif
	bool isCalculated = false;
	const Variables startVariables = variables;

	std::cout << "Start changing parameter " << parameterName << std::endl;
	std::cout << "Current value: " << parameter << ", target value: " << target << std::endl;
	std::cout << "Step: " << step << std::endl;
	std::cout << "////////////" << std::endl;

	while (variables.TAU >= MIN_RELAXATION_PARAMETER && !isCalculated)
	{
		try
		{
			if (target > parameter)
			{
				increaseParameter(runIterationProcess, variables, parameter, target, step, iterationsCounter,
					experimentsCounter, experimentVariables, iterationsInfo, drawRate);
			}
			else if (target < parameter)
			{
				decreaseParameter(runIterationProcess, variables, parameter, target, step, iterationsCounter,
					experimentsCounter, experimentVariables, iterationsInfo, drawRate);
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
	std::cout << "Finish changing parameter " << parameterName << std::endl;
	std::cout << "////////////" << std::endl << std::endl;
}

void algorithm::increaseParameter(void(*runIterationProcess) (Variables &, long long &) noexcept(false),
								  Variables &variables, double &parameter, double target, double step,
                                  long long &iterationsCounter, long long &experimentsCounter,
                                  std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                                  long long drawRate)
{
    bool isLastExperiment = false;

    while (parameter <= target)
    {
		if (parameter + step >= target)
		{
			isLastExperiment = true;
		}

        runExperiment(runIterationProcess, variables, iterationsCounter, experimentsCounter, 
					  experimentVariables, iterationsInfo, isLastExperiment, drawRate);

		if (isLastExperiment)
		{
			break;
		}

        parameter += step;
    }
}

void algorithm::decreaseParameter(void(*runIterationProcess) (Variables &, long long &) noexcept(false),
								  Variables &variables, double &parameter, double target, double step,
                                  long long &iterationsCounter, long long &experimentsCounter,
                                  std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                                  long long drawRate)
{
    bool isLastExperiment = false;

    while (parameter >= target)
    {
		if (parameter - step <= target)
		{
			isLastExperiment = true;
		}

        runExperiment(runIterationProcess, variables, iterationsCounter, experimentsCounter, 
					  experimentVariables, iterationsInfo, isLastExperiment, drawRate);

		if (isLastExperiment)
		{
			break;
		}

		parameter -= step;
    }
}


void algorithm::runExperiment(void(*runIterationProcess) (Variables &, long long &) noexcept(false), 
							  Variables &variables, long long &iterationsCounter, long long &experimentsCounter,
                              std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                              bool isLastExperiment, long long drawRate)
{
	runIterationProcess(variables, iterationsCounter);
	utils::clearProgressBar();

    pushExperimentResults(experimentsCounter, variables, experimentVariables, iterationsInfo, isLastExperiment, drawRate);

    experimentsCounter++;
}


void algorithm::pushExperimentResults(long long &experimentsCounter, Variables &variables,
                            std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                            bool isLastExperiment, long long drawRate)
{
    IterationInfo currentIterationInfo;
    Variables resultVariables = variables;

    convertToRadiusDimensionedVariables(resultVariables);

    experimentVariables.push_back(resultVariables);

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

        iterationsInfo.push_back(currentIterationInfo);
    }
}


void algorithm::convertToRadiusDimensionedVariables(Variables &variables)
{
    for (int i = 0; i < variables.r.size(); i++)
    {
        variables.r[i] *= variables.L;
        variables.z[i] *= variables.L;
    }
}


void algorithm::calcResult(void(*runIterationProcess) (Variables &, long long &) noexcept(false),
						   std::vector<Variables> &experimentVariables,
						   std::vector<IterationInfo> &iterationsInfo)
{
	Variables variables;
	variables.s = std::vector<double>(N + 1);
	variables.r = std::vector<double>(N + 1);
	variables.z = std::vector<double>(N + 1);
	variables.beta = std::vector<double>(N + 1);

	fillVariables(&variables);

	long long iterationsCounter = 0;
	long long experimentsCounter = 0;

	try
	{
// ***************U*****************
		changeParameter(runIterationProcess, variables, variables.U, 100, 10, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -2, "U");
// ***************A2*****************
		changeParameter(runIterationProcess, variables, variables.A2, 0.1, 0.05, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -2, "A2");
		
		changeParameter(runIterationProcess, variables, variables.A2, 0.5, 0.05, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -2, "A2");
		
		changeParameter(runIterationProcess, variables, variables.A2, 1, 0.05, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -2, "A2");
		
		changeParameter(runIterationProcess, variables, variables.A2, 1.5, 0.05, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -2, "A2");
		
		changeParameter(runIterationProcess, variables, variables.A2, 3.0, 0.05, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -1, "A2");
// ***************A1*****************
		changeParameter(runIterationProcess, variables, variables.A1, 3.0, 0.1, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -1, "A1");

		changeParameter(runIterationProcess, variables, variables.A1, 1.5, 0.1, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -1, "A1");

		changeParameter(runIterationProcess, variables, variables.A1, 1.0, 0.1, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -1, "A1");

		changeParameter(runIterationProcess, variables, variables.A1, 0.5, 0.1, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -1, "A1");

		changeParameter(runIterationProcess, variables, variables.A1, 0.0, 0.1, iterationsCounter,
			experimentsCounter, experimentVariables, iterationsInfo, -1, "A1");
		
		// changeParameter(runIterationProcess, variables, variables.A2, 6.0, 0.05, iterationsCounter,
		// 	experimentsCounter, experimentVariables, iterationsInfo, -1, "A2");
	}
	catch (ParameterNotReachTargetValue &e)
	{
		std::cerr << "Parameter " << e.getParameterName() << " didn't reach target value " 
				  << e.getTargetValue() << std::endl;
	}

#if LOG_RESULTS
	utils::printVector(std::cout, variables.s);
	std::cout << "************************" << std::endl;
	utils::printVector(std::cout, variables.r);
	std::cout << "************************" << std::endl;
	utils::printVector(std::cout, variables.z);
	std::cout << "************************" << std::endl;
	utils::printVector(std::cout, variables.beta);
	std::cout << "************************" << std::endl;
#endif


	std::cout << "iterations count: " << iterationsCounter << std::endl;
    std::cout << "experiments count: " << experimentsCounter << std::endl;
	std::cout << "************************" << std::endl;
}
