// Diploma.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <cmath>

const int N = 100;
const double LOWER_BOUND = 0;
const double UPPER_BOUND = 1;
const double STEP = (UPPER_BOUND - LOWER_BOUND) / N;


struct Variables
{
	std::vector<double> s;
	std::vector<double> r;
	std::vector<double> z;
	std::vector<double> beta;
};
