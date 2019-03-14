#include "Algorithm.h"
#include "../utils/Utils.h"
#include <iostream>


namespace algorithm
{
	void fillVariables(Variables *variables);

	void changeParameter(void(*runIterationProcess) (Variables &, long long &), 
						 Variables &variables, double &parameter, double target, double step,
                         long long &iterationsCounter, long long &experimentsCounter,
                         std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                         long long drawRate);

	void increaseParameter(void(*runIterationProcess) (Variables &, long long &), 
						   Variables &variables, double &parameter, double target, double step,
                           long long &iterationsCounter, long long &experimentsCounter,
                           std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                           long long drawRate);

	void decreaseParameter(void(*runIterationProcess) (Variables &, long long &), 
						   Variables &variables, double &parameter, double target, double step,
                           long long &iterationsCounter, long long &experimentsCounter,
                           std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                           long long drawRate);

    void runExperiment(void(*runIterationProcess) (Variables &, long long &), 
					   Variables &variables, long long &iterationsCounter, long long &experimentsCounter,
                       std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                       bool isLastExperiment, long long drawRate);

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
	variables->U = INITIAL_U;
	variables->B0 = INITIAL_B0;
	variables->A1 = INITIAL_A1;
	variables->A2 = INITIAL_A2;
	variables->ALPHA = INITIAL_ALPHA;
}


void algorithm::changeParameter(void (*runIterationProcess) (Variables &, long long &),
								Variables &variables, double &parameter, double target, double step,
                                long long &iterationsCounter, long long &experimentsCounter,
                                std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                                long long drawRate)
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
}

void algorithm::increaseParameter(void(*runIterationProcess) (Variables &, long long &), 
								  Variables &variables, double &parameter, double target, double step,
                                  long long &iterationsCounter, long long &experimentsCounter,
                                  std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                                  long long drawRate)
{
    bool isLastExperiment = false;
    const double halfStep = step / 2;

    while (parameter <= target)
    {
        runExperiment(runIterationProcess, variables, iterationsCounter, experimentsCounter, 
					  experimentVariables, iterationsInfo, isLastExperiment, drawRate);

		if (isLastExperiment)
		{
			break;
		}

        parameter += step;

		if (parameter >= target - halfStep)
		{
			isLastExperiment = true;
		}
    }
}

void algorithm::decreaseParameter(void(*runIterationProcess) (Variables &, long long &), 
								  Variables &variables, double &parameter, double target, double step,
                                  long long &iterationsCounter, long long &experimentsCounter,
                                  std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                                  long long drawRate)
{
    bool isLastExperiment = false;
	const double halfStep = step / 2;

    while (parameter >= target)
    {
        runExperiment(runIterationProcess, variables, iterationsCounter, experimentsCounter, 
					  experimentVariables, iterationsInfo, isLastExperiment, drawRate);

		if (isLastExperiment)
		{
			break;
		}

        parameter -= step;

		if (parameter <= target + halfStep)
		{
			isLastExperiment = true;
		}
    }
}


void algorithm::runExperiment(void(*runIterationProcess) (Variables &, long long &), 
							  Variables &variables, long long &iterationsCounter, long long &experimentsCounter,
                              std::vector<Variables> &experimentVariables, std::vector<IterationInfo> &iterationsInfo,
                              bool isLastExperiment, long long drawRate)
{
    runIterationProcess(variables, iterationsCounter);

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

    if (drawRate > 0 && experimentsCounter % drawRate == 0 || isLastExperiment)
    {
        currentIterationInfo.index = experimentsCounter;
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


void algorithm::calcResult(void(*runIterationProcess) (Variables &, long long &), 
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

	changeParameter(runIterationProcess, variables, variables.A2, 0.1, 0.05, iterationsCounter,
		experimentsCounter, experimentVariables, iterationsInfo, -1);
    
    changeParameter(runIterationProcess, variables, variables.A1, 6.0, 1.0, iterationsCounter,
                    experimentsCounter, experimentVariables, iterationsInfo, -1);

	changeParameter(runIterationProcess, variables, variables.A2, 1.5, 0.05, iterationsCounter,
	                experimentsCounter, experimentVariables, iterationsInfo, -1);

	 changeParameter(runIterationProcess, variables, variables.A2, 3.0, 0.05, iterationsCounter,
	                 experimentsCounter, experimentVariables, iterationsInfo, -1);
 
	changeParameter(runIterationProcess, variables, variables.A2, 6.0, 0.05, iterationsCounter,
					experimentsCounter, experimentVariables, iterationsInfo, -1);

	// changeParameter(runIterationProcess, variables, variables.U, 100.0, 10.0, iterationsCounter,
	// 				   experimentsCounter, experimentVariables, iterationsInfo, -1);

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
