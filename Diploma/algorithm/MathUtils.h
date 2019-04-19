#pragma once

#include <vector>


namespace algorithm
{
    double calcResidual(std::vector<double> &vector1, std::vector<double> &vector2) noexcept(false);

    bool isValid(std::vector<double> &vals);

    bool isValid(double val);
}
