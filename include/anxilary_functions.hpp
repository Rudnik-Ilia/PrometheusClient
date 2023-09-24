#pragma once

#include <chrono>

#include <iostream>


#define LOG(data) std::cout << data << std::endl

double TimeNow();

std::string DoubleToString(double num);

std::string Int64ToString(int64_t num);