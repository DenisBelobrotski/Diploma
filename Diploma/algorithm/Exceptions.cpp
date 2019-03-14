#include "Exceptions.h"

algorithm::InvalidResultException::InvalidResultException() : std::runtime_error("Result contains NaN or Infinity"){}

algorithm::IterationsLimitException::IterationsLimitException() : std::runtime_error("Iterations limit exceeded") {}
