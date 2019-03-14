#pragma once


#include "../Algorithm.h"


#define LOG_LOWER_PHI 0


namespace algorithm::e
{
	double calcLowerPhi(double r, double L, double A2);

	double calcLowerGamma(double r0, double r1, double I1, double I2, double L, 
						  double U, double B0, double A1, double ALPHA);

	double calcUpperPhi(double lowerGamma, double I1, double I2, double beta, double r, double z, double L, 
						double U, double B0, double A1, double A2);
}
