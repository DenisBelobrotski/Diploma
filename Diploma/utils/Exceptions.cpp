#include "Exceptions.h"

utils::exceptions::VectorSizeException::VectorSizeException() : runtime_error("VectorSizeException") {}

utils::exceptions::VectorSizeException::VectorSizeException(const char *msg) : runtime_error(msg) {}
