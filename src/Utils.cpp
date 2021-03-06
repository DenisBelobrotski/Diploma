#include "Utils.h"

#include <iostream>
#include <cmath>


void diploma::printVector(std::ostream& os, std::vector<double>& vector)
{
    for (const auto& currentComponent : vector)
    {
        os << currentComponent << std::endl;
    }
}


void diploma::pauseExecution()
{
#ifdef _MSC_VER
    system("pause");
#else
    std::cout << "Press enter to exit...";
    std::cin.clear();
    std::cin.get();
#endif
}


void diploma::showIterationsProgressBar(long long currentIteration, long long maxIteration)
{
    static auto divider = maxIteration / 10;
    if (currentIteration % divider == 0 && currentIteration != maxIteration)
    {
        showProgressBar((double) currentIteration / maxIteration);
    }

    if (currentIteration == maxIteration)
    {
        showProgressBar((double) currentIteration / maxIteration);
        clearProgressBar();
    }
}


void diploma::showProgressBar(double currentProgress)
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
    std::cout << "] " << (long long) (currentProgress * 100.0) << " %\r";
    std::cout.flush();
}


void diploma::terminateProgressBar()
{
    std::cout << std::endl;
    std::cout << "Calculations done!" << std::endl;
}


void diploma::clearProgressBar()
{
    std::cout << "\r";
    std::cout.flush();
}
