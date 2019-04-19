#pragma once

#include <stdexcept>
#include <string>


namespace algorithm
{

    class InvalidResultException : public std::runtime_error
    {
    public:
        InvalidResultException();
    };

    class IterationsLimitException : public std::runtime_error
    {
    public:
        IterationsLimitException();
    };

    class ParameterNotReachTargetValue : public std::runtime_error
    {
    private:
        std::string parameterName;
        double targetValue;
    public:
        ParameterNotReachTargetValue(std::string parameterName, double targetValue);

        std::string getParameterName();

        double getTargetValue();
    };

    class VectorSizeException : public std::runtime_error
    {

    public:
        VectorSizeException();

        explicit VectorSizeException(const char *msg);

    };
}
