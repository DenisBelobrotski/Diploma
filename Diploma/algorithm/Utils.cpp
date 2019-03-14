#include "Utils.h"
#include <cmath>


bool algorithm::utils::isValid(std::vector<double> &vals)
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


bool algorithm::utils::isValid(double val)
{
    if (!std::isfinite(val))
    {
        return false;
    }
    return true;
}
