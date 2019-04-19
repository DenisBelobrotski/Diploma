#include "Exceptions.h"

algorithm::InvalidResultException::InvalidResultException() :
        std::runtime_error("Result contains NaN or Infinity") {}

algorithm::IterationsLimitException::IterationsLimitException() :
        std::runtime_error("Iterations limit exceeded") {}

algorithm::ParameterNotReachTargetValue::ParameterNotReachTargetValue(std::string parameterName, double targetValue) :
        std::runtime_error("Parameter didn't reach target value"), parameterName(parameterName),
        targetValue(targetValue) {}

std::string algorithm::ParameterNotReachTargetValue::getParameterName()
{
    return parameterName;
}

double algorithm::ParameterNotReachTargetValue::getTargetValue()
{
    return targetValue;
}

algorithm::VectorSizeException::VectorSizeException() : runtime_error("VectorSizeException") {}

algorithm::VectorSizeException::VectorSizeException(const char *msg) : runtime_error(msg) {}
