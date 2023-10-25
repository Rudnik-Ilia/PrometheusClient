#pragma once

#include <chrono>
#include <iostream>

#define LOG(data) std::cout << data << std::endl

/**
 * @brief Adds quotes;
 *
 * @param any std::string.
 * @return std::string.
 */
std::string AddQuotes(std::string str);