//
// Created by user on 2019-02-27.
//

#ifndef DIPLOMA_PLOT_EXCEPTIONS_H
#define DIPLOMA_PLOT_EXCEPTIONS_H

#include <stdexcept>


namespace plot::exceptions
{

    class PipeException : public std::runtime_error
    {

    public:
        PipeException();
        explicit PipeException(const char * msg);

    };

    class VectorSizeException : public std::runtime_error
    {

    public:
        VectorSizeException();
        explicit VectorSizeException(const char * msg);

    };

}


#endif //DIPLOMA_PLOT_EXCEPTIONS_H
