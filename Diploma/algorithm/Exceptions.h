#pragma once

#include <stdexcept>


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
}
