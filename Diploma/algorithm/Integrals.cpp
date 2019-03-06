#include "Integrals.h"
#include "Formulas.h"


double algorithm::calcIntegral0(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpZ = (variables->z[i - 1] + variables->z[i]) / 2;
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		result += tmpZ * tmpR * cos(tmpBeta);
	}

	return 2 * M_PI * STEP * result;
}


double algorithm::calcIntegral1(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpZ = (variables->z[i - 1] + variables->z[i]) / 2;
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		double lowerPhi = calcLowerPhi(tmpR, variables->L);
		result += lowerPhi * tmpZ * tmpR * cos(tmpBeta);
	}

	return 2 * M_PI * STEP * result;
}


double algorithm::calcIntegral2(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		double lowerPhi = calcLowerPhi(tmpR, variables->L);
		result += lowerPhi * tmpR * cos(tmpBeta);
	}

	return STEP * result;
}
