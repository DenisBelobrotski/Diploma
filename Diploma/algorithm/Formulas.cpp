#include "Formulas.h"
#include <math.h>
#include "Algorithm.h"


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


double algorithm::calcL(double I0)
{
	return sgn(I0) * pow(U / fabs(I0), 1.0 / 3);
}


double algorithm::calcLowerPhi(double r, double L)
{
	if (A2 == 0.0)
	{
		return 1;
	}
	double tmp = r * L / A2;

	double result = tmp * sinh(1 / tmp);

	return result;
}


double algorithm::calcLowerGamma(double r0, double r1, double I1, double I2, double L)
{
	return (2 / (r1 * r1 - r0 * r0)) * (r0 * cos(ALPHA) - r1 * sin(ALPHA) - (B0 * U) / (2 * M_PI * L) + (A1 * U * I2) / (I1 * L * L));
}


double algorithm::calcUpperPhi(double lowerGamma, double I1, double I2, double beta, double r, double z, double L)
{
	return B0 * z * L * L - (A1 * U * calcLowerPhi(r, L)) / (I1 * L * L) - sin(beta) / r + lowerGamma;
}
