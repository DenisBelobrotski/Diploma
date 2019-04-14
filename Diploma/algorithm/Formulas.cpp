#include "Formulas.h"
#include "DifferenceMethod.h"
#include <cmath>


int algorithm::sgn(double value)
{
	if (value < 0)
	{
		return -1;
	}

	if (value > 0)
	{
		return 1;
	}

	return 0;
}


double algorithm::calcL(double I0, double U)
{
	return sgn(I0) * pow(U / fabs(I0), 1.0 / 3);
}


void algorithm::convertLengthToRadiusDimensionedVariables(Variables &variables)
{
	for (int i = 0; i < variables.r.size(); i++)
	{
		variables.r[i] *= variables.L;
		variables.z[i] *= variables.L;
	}
}
