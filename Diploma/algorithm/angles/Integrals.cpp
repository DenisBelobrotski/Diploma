#include "Integrals.h"
#include "../Algorithm.h"
#include "Formulas.h"
#include "../Formulas.h"
#include <iostream>
#include <math.h>


double algorithm::e::calcIntegral0(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpZ = (variables->z[i - 1] + variables->z[i]) / 2;
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		double tmp = tmpZ * tmpR * cos(tmpBeta);
		result += tmp;
#if LOG_INTEGRAL_0
		std::cout << "tmpR: " << tmpR << " tmpZ: " << tmpZ << std::endl;
		std::cout << "tmpBeta: " << tmpBeta << std::endl;
		std::cout << "tmp: " << tmp << std::endl;
		std::cout << "*********" << std::endl;
#endif
	}

#if LOG_INTEGRAL_0
	std::cout << "result: " << result << std::endl;
	std::cout << "************" << std::endl;
#endif

	result *= 2 * M_PI * STEP;

#if LOG_INTEGRAL_0
	std::cout << "final result: " << result << std::endl;
	std::cout << "************" << std::endl;
#endif

	return result;
}


double algorithm::e::calcIntegral1(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpZ = (variables->z[i - 1] + variables->z[i]) / 2;
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		double lowerPhi = calcLowerPhi(tmpR, variables->L, variables->A2);
		double tmp = lowerPhi * tmpZ * tmpR * cos(tmpBeta);
		result += tmp;
#if LOG_INTEGRAL_1
		std::cout << "tmpR: " << tmpR << " tmpZ: " << tmpZ << std::endl;
		std::cout << "tmpBeta: " << tmpBeta << " lowerPhi: " << lowerPhi << std::endl;
		std::cout << "tmp: " << tmp << std::endl;
		std::cout << "*********" << std::endl;
#endif
	}

#if LOG_INTEGRAL_1
	std::cout << "result: " << result << std::endl;
	std::cout << "************" << std::endl;
#endif

	result *= 2 * M_PI * STEP;

#if LOG_INTEGRAL_1
	std::cout << "final result: " << result << std::endl;
	std::cout << "************" << std::endl;
#endif

	return result;
}


double algorithm::e::calcIntegral2(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		double lowerPhi = calcLowerPhi(tmpR, variables->L, variables->A2);
		double tmp = lowerPhi * tmpR * cos(tmpBeta);
		result += tmp;
#if LOG_INTEGRAL_2
		std::cout << "tmpR: " << tmpR << std::endl;
		std::cout << "tmpBeta: " << tmpBeta << " lowerPhi: " << lowerPhi << std::endl;
		std::cout << "tmp: " << tmp << std::endl;
		std::cout << "*********" << std::endl;
#endif
	}

#if LOG_INTEGRAL_2
	std::cout << "result: " << result << std::endl;
	std::cout << "************" << std::endl;
#endif

	result *= STEP;

#if LOG_INTEGRAL_2
	std::cout << "final result: " << result << std::endl;
	std::cout << "************" << std::endl;
#endif

	return result;
}
