#include "Formulas.h"
#include "../Algorithm.h"
#include <iostream>
#include <math.h>


double algorithm::e::calcLowerPhi(double r, double L, double A2)
{
#if CALC_WITHOUT_CONCENTRATION
	return 1;
#endif
	if (A2 == 0.0)
	{
		return 1;
	}
	double tmp = r * L / A2;

	double invertTmp = 1 / tmp;

	double sinH = sinh(invertTmp);

	double result = tmp * sinH;

#if LOG_LOWER_PHI
	std::cout << "r: " << r << " L: " << L << " A2: " << A2 << std::endl;
	std::cout << "tmp: " << tmp << " invertTmp: " << invertTmp << " sinH: " << sinH << std::endl;
	std::cout << "result: " << result << std::endl;
	std::cout << "************" << std::endl;
#endif

	return result;
}


double algorithm::e::calcLowerGamma(double r0, double r1, double I1, double I2, double L,
								 double U, double B0, double A1, double ALPHA)
{
	return (2 / (r1 * r1 - r0 * r0)) * (r0 * cos(ALPHA) - r1 * sin(ALPHA) - (B0 * U) / (2 * M_PI * L) + (A1 * U * I2) / (I1 * L * L));
}


double algorithm::e::calcUpperPhi(double lowerGamma, double I1, double I2, double beta, double r, double z, double L,
							   double U, double B0, double A1, double A2)
{
	return B0 * z * L * L - (A1 * U * calcLowerPhi(r, L, A2)) / (I1 * L * L) - sin(beta) / r + lowerGamma;
}
