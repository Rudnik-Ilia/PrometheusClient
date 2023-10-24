#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>

#include "../include/init.hpp"
#include "../include/prometheus.hpp"
#include "../include/generator.hpp"

using vectorStr = std::vector<std::string>;
template<class T>
using Buckets = std::vector<T>;



int main()
{
    Generator gen(5);
    
    for(const auto& iter : gen)
    {
        LOG(iter);
    }
    return 0;
}