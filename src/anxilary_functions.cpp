#include <iostream>

#include "../include/anxilary_functions.hpp"

#define LOG(data) std::cout << data << std::endl

double TimeNow()
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::chrono::seconds::period>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

std::string DoubleToString(double num)
{
    return std::to_string(num);
}

std::string Int64ToString(int64_t num)
{
    return std::to_string(num);
}

