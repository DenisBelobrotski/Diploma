#include "Exceptions.h"


diploma::IncorrectConfigException::IncorrectConfigException() : runtime_error("IncorrectConfigException") {}

diploma::IncorrectConfigException::IncorrectConfigException(const char* msg) : runtime_error(msg) {}
