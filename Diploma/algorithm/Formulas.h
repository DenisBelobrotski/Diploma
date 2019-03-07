#pragma once


namespace algorithm
{
	int sgn(double value);

	double calcL(double I0);

	double calcLowerPhi(double r, double L);

	double calcLowerGamma(double r0, double r1, double I1, double I2, double L);

	double calcUpperPhi(double lowerGamma, double I1, double I2, double beta, double r, double z, double L);
}
