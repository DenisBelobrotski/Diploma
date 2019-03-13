#include "Utils.h"
#include "Exceptions.h"
#include <iostream>
#include <cfloat>
#include <cmath>
#include <algorithm>
#include <sstream>

void utils::printVector(std::ostream &os, std::vector<double> &vector)
{
	for (const auto& currentComponent : vector)
	{
		os << currentComponent << std::endl;
	}
}


double utils::calcResidual(std::vector<double> &vector1, std::vector<double> &vector2) noexcept(false)
{
	auto size1 = vector1.size();
	auto size2 = vector2.size();
	if (size1 != size2)
	{
		std::stringstream msgStream;
		msgStream << "Components vectors sizes isn't equal: vector1 size: " << size1
				  << ", vector2 size: " << size2;

		throw exceptions::VectorSizeException(msgStream.str().c_str());
	}

	auto result = DBL_MIN;

	for (int i = 0; i < vector1.size(); i++)
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