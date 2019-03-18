#include "Utils.h"
#include "Exceptions.h"
#include <iostream>
#include <cfloat>
#include <cmath>
#include <algorithm>
#include <sstream>
#include "../algorithm/Algorithm.h"


void utils::printVector(std::ostream &os, std::vector<double> &vector)
{
	for (const auto& currentComponent : vector)
	{
		os << currentComponent << std::endl;
	}
}


double utils::calcResidual(std::vector<double> &vector1, std::vector<double> &vector2) noexcept(false)
{
	const auto size1 = vector1.size();
	const auto size2 = vector2.size();
	if (size1 != size2)
	{
		std::stringstream msgStream;
		msgStream << "Components vectors sizes isn't equal: vector1 size: " << size1
				  << ", vector2 size: " << size2;

		throw exceptions::VectorSizeException(msgStream.str().c_str());
	}

	auto result = DBL_MIN;

	for (auto i = 0; i < vector1.size(); i++)
	{
		result = std::max(result, fabs(vector1[i] - vector2[i]));
	}

	return result;
}


void utils::pauseExecution()
{
#ifdef _MSC_VER
	system("pause");
#else
	std::cout << "Press enter to exit...";
	std::cin.clear();
	std::cin.get();
#endif
}


bool utils::isValid(std::vector<double> &vals)
{
	for (auto& val : vals)
	{
		if (!std::isfinite(val))
		{
			return false;
		}
	}
	return true;
}


bool utils::isValid(double val)
{
	return !std::isfinite(val);
}


void utils::showIterationsProgressBar(long long currentIteration, long long maxIteration)
{
	static int dividor = algorithm::MAX_ITERATIONS_NUMBER / 10;
    if (currentIteration % dividor == 0 && currentIteration != maxIteration)
    {
        showProgressBar((double) currentIteration / maxIteration);
    }

	if (currentIteration == maxIteration)
	{
        showProgressBar((double) currentIteration / maxIteration);
		clearProgressBar();
	}
}


void utils::showProgressBar(double currentProgress)
{
	int barWidth = 70;
	long long pos = std::llround(barWidth * currentProgress);

	std::cout << "[";
	for (int i = 0; i < barWidth; ++i)
	{
		if (i < pos)
		{
			std::cout << "=";
		}
		else if (i == pos)
		{
			std::cout << ">";
		}
		else
		{
			std::cout << " ";
		}
	}
	std::cout << "] " << (long long)(currentProgress * 100.0) << " %\r";
	std::cout.flush();
}


void utils::terminateProgressBar()
{
	std::cout << std::endl;
    std::cout << "Calculations done!" << std::endl;
}


void utils::clearProgressBar()
{
	std::cout << "\r";
	std::cout.flush();
}
