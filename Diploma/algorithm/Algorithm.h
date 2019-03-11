#pragma once


#define _USE_MATH_DEFINES


#include <vector>
#include <math.h>


#define LOG_BETA 0
#define LOG_RESULTS 0

#define SIMPLE_RELAXATION_FORMULA 1


namespace algorithm
{
	const int N = 100;
	const double LOWER_BOUND = 0;
	const double UPPER_BOUND = 1;
	const double STEP = (UPPER_BOUND - LOWER_BOUND) / N;
	const double TAU = 0.1;
	const double ACCURACY = 1E-5;

	inline double U = 3; //400
    inline double B0 = 1; //1
    inline double A1 = 60; //6
    inline double A2 = 0.05; //0, 1, 3, 6
    inline double ALPHA = M_PI_4; //M_PI_4, M_PI_2


	struct Variables
	{
		std::vector<double> s;
		std::vector<double> r;
		std::vector<double> z;
		std::vector<double> beta;
		double L;
	};

	void calcResult(std::vector<Variables> &experimentVariables, std::vector<int> &experimentNumbers);
}
