#pragma once


#include <vector>
#include <ostream>


namespace utils
{
    void printVector(std::ostream &os, std::vector<double> &vector);

    double calcResidual(std::vector<double> &vector1, std::vector<double> &vector2) noexcept(false);

    void pauseExecution();

    bool isValid(std::vector<double> &vals);

    bool isValid(double val);

    void showIterationsProgressBar(long long currentIteration, long long maxIteration);

    void showProgressBar(double currentProgress);

    void terminateProgressBar();

    void clearProgressBar();
}
