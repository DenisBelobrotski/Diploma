#include "Algorithm.h"
#include "../utils/Utils.h"
#include "Formulas.h"
#include "Integrals.h"

#include <iostream>


namespace algorithm
{
	void fillVariables(Variables *variables);

	void calcBeta(Variables *variables, Variables *prevVariables);

	void calcRadius(Variables *variables);

	void calcHeight(Variables *variables);

	void calcIteration(Variables *variables, Variables *prevVariables);
}


void algorithm::fillVariables(Variables *variables)
{
	for (int i = 0; i < N + 1; i++)
	{
		variables->s[i] = LOWER_BOUND + i * STEP;

		variables->r[i] = variables->s[i];

		variables->z[N - i] = variables->s[i];

		variables->beta[i] = -M_PI_4;
	}
}


void algorithm::calcBeta(Variables *variables, Variables *prevVariables)
{
	double r0 = variables->r[0];
	double r1 = variables->r[N];

	double I0 = calcIntegral0(variables);
	double L = variables->L = calcL(I0);
	double I1 = calcIntegral1(variables);
	double I2 = calcIntegral2(variables);

	double lowerGamma = calcLowerGamma(r0, r1, I1, I2, L);

	for (int i = N - 1; i > 0; i--)
	{
		double tmpZ = (variables->z[i] + variables->z[i + 1]) / 2;
		double tmpR = (variables->r[i] + variables->r[i + 1]) / 2;
		double tmpBeta = (variables->beta[i] + variables->beta[i + 1]) / 2;

		double upperPhi = calcUpperPhi(lowerGamma, I1, I2, tmpBeta, tmpR, tmpZ, L);

		variables->beta[i] = variables->beta[i + 1] - STEP * upperPhi +
			(1 - TAU) * (variables->beta[i] - variables->beta[i + 1] + STEP * upperPhi);
	}
}


void algorithm::calcRadius(Variables *variables)
{
	variables->r[0] = 1 / variables->L;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		variables->r[i] = variables->r[i - 1] + STEP * cos(tmpBeta);
	}
}


void algorithm::calcHeight(Variables *variables)
{
	variables->z[N] = 0;

	for (int i = N - 1; i >= 0; i--)
	{
		double tmpBeta = (variables->beta[i] + variables->beta[i + 1]) / 2;
		variables->z[i] = variables->z[i + 1] - STEP * sin(tmpBeta);
	}
}


void algorithm::calcIteration(Variables *variables, Variables *prevVariables)
{
	calcBeta(variables, prevVariables);
	calcRadius(variables);
	calcHeight(variables);
}


void algorithm::calcResult(std::vector<Variables> &iterationVariables, std::vector<int> &iterationNumbers)
{
	Variables variables0;
	variables0.s = std::vector<double>(N + 1);
	variables0.r = std::vector<double>(N + 1);
	variables0.z = std::vector<double>(N + 1);
	variables0.beta = std::vector<double>(N + 1);

	fillVariables(&variables0);

	for (int i = 0; i < 1; i++)
	{
		calcIteration(&variables0, nullptr);
	}

	utils::printVector(&std::cout, &(variables0.s));
	std::cout << "************************" << std::endl;
	utils::printVector(&std::cout, &(variables0.r));
	std::cout << "************************" << std::endl;
	utils::printVector(&std::cout, &(variables0.z));
	std::cout << "************************" << std::endl;
	utils::printVector(&std::cout, &(variables0.beta));
	std::cout << "************************" << std::endl;

	iterationVariables.push_back(variables0);

	iterationNumbers.push_back(0);

	// for (int i = 1; i < 2; i++)
	// {
	// 	Variables variables;
	// 	variables.s = std::vector<double>(N + 1);
	// 	variables.r = std::vector<double>(N + 1);
	// 	variables.z = std::vector<double>(N + 1);
	// 	variables.beta = std::vector<double>(N + 1);
	//
	// 	calcIteration(&variables, &(iterationVariables[i - 1]));
	//
	// 	iterationVariables.push_back(variables);
	//
	// 	printVector(&std::cout, &(variables.s));
	// 	std::cout << "************************" << std::endl;
	// 	printVector(&std::cout, &(variables.r));
	// 	std::cout << "************************" << std::endl;
	// 	printVector(&std::cout, &(variables.z));
	// 	std::cout << "************************" << std::endl;
	// 	printVector(&std::cout, &(variables.beta));
	// 	std::cout << "************************" << std::endl;
	// }
	//
	// iterationNumbers.push_back(1);
}
