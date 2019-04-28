#pragma once

#include <stdexcept>
#include <string>


namespace diploma
{
    class IncorrectConfigException : public std::runtime_error
    {
    public:
        IncorrectConfigException();

        explicit IncorrectConfigException(const char* msg);
    };
}
