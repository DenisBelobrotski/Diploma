#pragma once


#include <vector>
#include <ostream>


namespace utils
{
    void printVector(std::ostream &os, std::vector<double> &vector);

    void pauseExecution();

    void showIterationsProgressBar(long long currentIteration, long long maxIteration);

    void showProgressBar(double currentProgress);

    void terminateProgressBar();

    void clearProgressBar();
}
