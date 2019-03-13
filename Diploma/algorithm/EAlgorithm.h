// Algorithm based on "Euler method" (it looks like Euler method)

#pragma once

#include "Algorithm.h"

#define LOG_BETA 0

#define SIMPLE_RELAXATION_FORMULA 1

namespace algorithm::e
{
	void runIterationProcess(Variables &variables, long long &iterationsCounter);
}
