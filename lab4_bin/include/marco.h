#pragma once

#include <iostream>

#define assert_true_return(condition, message) \
{\
    if (!(condition)) {\
        std::cout << message << std::endl;\
        return false;\
    }\
}

#define assert_false_return(condition, message) \
{\
    if (condition) {\
        std::cout << message << std::endl;\
        return false;\
    }\
}
