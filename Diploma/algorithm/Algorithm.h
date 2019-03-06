#pragma once


#define _USE_MATH_DEFINES


#include <vector>
#include <math.h>


namespace algorithm
{
	const int N = 1000;
	const double LOWER_BOUND = 0;
	const double UPPER_BOUND = 1;
	const double STEP = (UPPER_BOUND - LOWER_BOUND) / N;
	const double TAU = 1;

	const double U = 400;
	const double B0 = 1;
	const double A1 = 6;
	const double A2 = 3; //0, 1, 3, 6
	const double ALPHA = M_PI_4; //M_PI_4, M_PI_2


	struct Variables
	{
		std::vector<double> s;
		std::vector<double> r;
		std::vector<double> z;
		std::vector<double> beta;
		double L;
	};

	void calcResult(std::vector<Variables> &iterationVariables, std::vector<int> &iterationNumbers);
}
