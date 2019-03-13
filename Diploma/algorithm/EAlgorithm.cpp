#include "EAlgorithm.h"
#include "Algorithm.h"
#include "Formulas.h"
#include "Integrals.h"
#include "../utils/Utils.h"
#include <iostream>
#include <cmath>
#include <algorithm>


namespace algorithm::e
{
	void calcBeta(Variables *variables);

	void calcRadius(Variables *variables);

	void calcHeight(Variables *variables);

	void calcIteration(Variables *variables);
}


void algorithm::e::calcBeta(Variables *variables)
{
	double r0 = variables->r[0];
	double r1 = variables->r[N];

	double I0 = calcIntegral0(variables);
	double L = variables->L = calcL(I0, variables->U);
	double I1 = calcIntegral1(variables);
	double I2 = calcIntegral2(variables);

	if (r0 != r0 || r1 != r1 || I0 != I0 || L != L || I1 != I1 || I2 != I2)
	{
		std::cerr << "r0 || r1 || I0 || L || I1 || I2 is nan" << std::endl;
		utils::pauseExecution();
	}

	double lowerGamma = calcLowerGamma(r0, r1, I1, I2, L,
		variables->U, variables->B0,
		variables->A1, variables->ALPHA);

#if LOG_BETA
	std::cout << "r0: " << r0 << " r1: " << r1 << std::endl;
	std::cout << "I0: " << I0 << " L: " << L << std::endl;
	std::cout << "I1: " << I1 << " I2: " << I2 << std::endl;
	std::cout << "*****************" << std::endl;
#endif

#if SIMPLE_RELAXATION_FORMULA
	std::vector<double> prevBeta = variables->beta;
#endif

	for (int i = N - 1; i > 0; i--)
	{
		double tmpZ = (variables->z[i] + variables->z[i + 1]) / 2;
		double tmpR = (variables->r[i] + variables->r[i + 1]) / 2;
		double tmpBeta = (variables->beta[i] + variables->beta[i + 1]) / 2;

		double upperPhi = calcUpperPhi(lowerGamma, I1, I2, tmpBeta, tmpR, tmpZ, L,
			variables->U, variables->B0, variables->A1, variables->A2);

#if SIMPLE_RELAXATION_FORMULA
		variables->beta[i] = variables->beta[i + 1] - STEP * upperPhi;
		if (variables->beta[i] != variables->beta[i])
		{
			std::cerr << "beta is nan" << std::endl;
			utils::pauseExecution();
		}
#else
		variables->beta[i] = variables->beta[i + 1] - STEP * upperPhi +
			(1 - TAU) * (variables->beta[i] - variables->beta[i + 1] + STEP * upperPhi);
#endif

#if LOG_BETA
		std::cout << "tmpR: " << tmpR << " tmpZ: " << tmpZ << std::endl;
		std::cout << "tmpBeta: " << tmpBeta << " upperPhi: " << upperPhi << std::endl;
		std::cout << "newBeta: " << variables->beta[i] << std::endl;
		std::cout << "**********" << std::endl;
#endif
	}

#if SIMPLE_RELAXATION_FORMULA
	for (int i = 0; i < N + 1; i++)
	{
		variables->beta[i] = (1 - TAU) * prevBeta[i] + TAU * variables->beta[i];
	}
#endif
}


void algorithm::e::calcRadius(Variables *variables)
{
	variables->r[0] = 1 / variables->L;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		variables->r[i] = variables->r[i - 1] + STEP * cos(tmpBeta);
	}
}


void algorithm::e::calcHeight(Variables *variables)
{
	variables->z[N] = 0;

	for (int i = N - 1; i >= 0; i--)
	{
		double tmpBeta = (variables->beta[i] + variables->beta[i + 1]) / 2;
		variables->z[i] = variables->z[i + 1] - STEP * sin(tmpBeta);
	}
}


void algorithm::e::calcIteration(Variables *variables)
{
	calcBeta(variables);
	calcRadius(variables);
	calcHeight(variables);
}


void algorithm::e::runIterationProcess(Variables &variables, long long &iterationsCounter)
{
	double residual;

	std::vector<double> prevR;
	std::vector<double> prevZ;

	do
	{
		prevR = variables.r;
		prevZ = variables.z;

		calcIteration(&variables);

		residual = std::max(utils::calcResidual(prevR, variables.r),
			utils::calcResidual(prevZ, variables.z));

		iterationsCounter++;
	} while (residual > algorithm::ACCURACY);
}
