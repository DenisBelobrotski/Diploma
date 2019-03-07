//
// Created by user on 2019-02-27.
//

#include "Exceptions.h"


plot::exceptions::PipeException::PipeException() : runtime_error("PipeException") {}

plot::exceptions::PipeException::PipeException(const char * msg) : runtime_error(msg) {}

plot::exceptions::VectorSizeException::VectorSizeException() : runtime_error("VectorSizeException") {}

plot::exceptions::VectorSizeException::VectorSizeException(const char * msg) : runtime_error(msg) {}
