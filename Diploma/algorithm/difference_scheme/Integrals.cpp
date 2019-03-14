#include "Integrals.h"
#include "Formulas.h"
#include <math.h>
#include <iostream>


double algorithm::ds::calcIntegral0(Variables *variables)
{
	double sum = 0;

	for (auto i = 1; i < N + 1; i++)
	{
		double height = (variables->z[i] + variables->z[i - 1]) / 2;
		double radiusQuadDiff = variables->r[i] * variables->r[i] -
								variables->r[i - 1] * variables->r[i - 1];

		sum += height * radiusQuadDiff;
	}

	return M_PI * sum;
}

double algorithm::ds::calcIntegral1(Variables *variables)
{
	double sum = 0;

	for (auto i = 1; i < N + 1; i++)
	{
		double height = (variables->z[i] + variables->z[i - 1]) / 2;
		double radiusQuadDiff = variables->r[i] * variables->r[i] -
								variables->r[i - 1] * variables->r[i - 1];
		double lowerPhi = calcLowerPhi(i, variables);

		sum += height * radiusQuadDiff * lowerPhi;
	}

	return M_PI * sum;
}

double algorithm::ds::calcIntegral2(Variables *variables)
{
	double sum = 0;

	for (auto i = 1; i < N + 1; i++)
	{
		double radiusQuadDiff = variables->r[i] * variables->r[i] -
								variables->r[i - 1] * variables->r[i - 1];
		double lowerPhi = calcLowerPhi(i, variables);

		sum += radiusQuadDiff * lowerPhi;
	}

	return sum / 2;
}
