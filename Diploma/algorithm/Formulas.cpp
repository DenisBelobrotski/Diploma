#include "Formulas.h"
#include "Algorithm.h"
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
