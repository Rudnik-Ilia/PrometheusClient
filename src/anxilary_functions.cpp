#include <iostream>

#include "../include/anxilary_functions.hpp"


std::string AddQuotes(std::string str)
{
    std::string res;
    res.reserve(str.size() + 2);
    res += '\"';
    res += str;
    res += '\"';

    return res;
}
