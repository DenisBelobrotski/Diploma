#include <sstream>
#include <cfloat>
#include <cmath>

#include <MagneticFluidFormAlgorithm/MagneticFluidFormAlgorithm.h>


double algorithm::calcResidual(std::vector<double> &vector1, std::vector<double> &vector2) noexcept(false)
{
    const auto size1 = vector1.size();
    const auto size2 = vector2.size();
    if (size1 != size2)
    {
        std::stringstream msgStream;
        msgStream << "Components vectors sizes isn't equal: vector1 size: " << size1
                  << ", vector2 size: " << size2;

        throw VectorSizeException(msgStream.str().c_str());
    }

    auto result = DBL_MIN;

    for (auto i = 0; i < vector1.size(); i++)
    {
        result = std::max(result, fabs(vector1[i] - vector2[i]));
    }

    return result;
}


bool algorithm::isValid(std::vector<double> &vals)
{
    for (auto &val : vals)
    {
        if (!std::isfinite(val))
        {
            return false;
        }
    }
    return true;
}


bool algorithm::isValid(double val)
{
    return !std::isfinite(val);
}
