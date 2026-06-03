#include "../include/anxilary_functions.hpp"

std::string AddQuotes(std::string str)
{
    std::string result;
    result.reserve(str.size() + 2);
    result += '"';
    result += str;
    result += '"';
    return result;
}
