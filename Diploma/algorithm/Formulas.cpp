#include "Formulas.h"
#include <math.h>
#include "Algorithm.h"
#include <iostream>


#pragma mark - Algorithm

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


#pragma mark - Angles algorithm

double algorithm::e::calcLowerPhi(double r, double L, double A2)
{
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



#pragma mark - Difference scheme algorithm

double algorithm::ds::calcLowerPhi(int i, Variables *variables)
{
    if (i == 0)
    {
        return sinh(variables->A2) / variables->A2;
    }

    double tmp = variables->L * variables->r[i] / variables->A2;
    return tmp * sinh(1 / tmp);
}


double algorithm::ds::calcLowerGamma(Variables *variables, double I1, double I2)
{
	double sum = variables->r[0] * cos(INITIAL_ALPHA) - variables->r[N] * sin(INITIAL_ALPHA) -
				 variables->B0 * variables->U / (2 * M_PI * variables->L) +
				 variables->A1 * variables->U * I2 / (I1 * variables->L * variables->L);

	return 2 * sum / (variables->r[N] * variables->r[N] -
					  variables->r[0] * variables->r[0]);
}


double algorithm::ds::calcUpperPhi(int i, Variables *variables, double lowerGamma, double I1)
{
    double sum = -variables->A1 * variables->U * calcLowerPhi(i, variables) / (I1 * variables->L * variables->L) + lowerGamma;

    if (i == 0)
    {
        return variables->B0 * variables->L * variables->L * variables->z[i] + sum +
               variables->L * cos(INITIAL_ALPHA);
    }

    if (i == N)
    {
        return sum + sin(INITIAL_ALPHA) / variables->r[i];
    }

    return variables->B0 * variables->L * variables->L * variables->z[i] + sum -
           ((variables->z[i + 1] - variables->z[i - 1]) / (2 * STEP * variables->r[i]));
}
