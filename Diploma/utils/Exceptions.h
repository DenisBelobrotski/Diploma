#pragma once

#include <stdexcept>


namespace utils::exceptions
{
    class VectorSizeException : public std::runtime_error
    {

    public:
        VectorSizeException();
        explicit VectorSizeException(const char * msg);

    };
}
