#include "Utils.h"
#include <iostream>


void utils::printVector(std::ostream *os, std::vector<double> *vector)
{
	for (const auto& currentComponent : *vector)
	{
		*os << currentComponent << std::endl;
	}
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