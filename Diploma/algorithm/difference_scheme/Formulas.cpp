#include "Formulas.h"
#include <math.h>
#include "../Algorithm.h"
#include <iostream>


double algorithm::ds::calcLowerPhi(int i, Variables *variables)
{
#if CALC_WITHOUT_CONCENTRATION
	return 1;
#endif
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
