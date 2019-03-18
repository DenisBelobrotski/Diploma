#pragma once


#define _USE_MATH_DEFINES


#include <vector>
#include <math.h>


#define LOG_RESULTS 0
#define ALWAYS_RESET_TAU 0


namespace algorithm
{
	const int N = 100;
	const double LOWER_BOUND = 0;
	const double UPPER_BOUND = 1;
	const double STEP = (UPPER_BOUND - LOWER_BOUND) / N;
	const double ACCURACY = 1E-5;
	const int MAX_ITERATIONS_NUMBER = 50000;
	const double MIN_RELAXATION_PARAMETER = 0.05;
	const double RELAXATION_MULTIPLIER = 0.5;

	const double INITIAL_TAU = 1; //1
	const double INITIAL_U = 3; //400
    const double INITIAL_B0 = 1; //1
    const double INITIAL_A1 = 6; //6
    const double INITIAL_A2 = 0.05; //0, 1, 3, 6
    const double INITIAL_ALPHA = M_PI_4; //M_PI_4, M_PI_2


	struct Variables
	{
		std::vector<double> s;
		std::vector<double> r;
		std::vector<double> z;
		std::vector<double> beta;
		double L;
		double TAU;
		double U;
		double B0;
		double A1;
		double A2;
		double ALPHA;
	};


	struct IterationInfo
	{
		long long index;
		double tau;
		double u;
		double b0;
		double a1;
		double a2;
		double alpha;
	};

	void calcResult(void(*runIterationProcess) (Variables &, long long &) noexcept(false),
					std::vector<Variables> &experimentVariables,
					std::vector<IterationInfo> &iterationsInfo);
}
