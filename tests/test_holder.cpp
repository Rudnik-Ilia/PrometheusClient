#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>
#include <typeinfo>

#include "../include/init.hpp"
#include "../include/prometheus.hpp"
#include "../include/holder.hpp"
#include "../include/prometheus.hpp"

using vectorStr = std::vector<std::string>;
template<class T>
using Buckets = std::vector<T>;


int main()
{
    // auto& holder = Holder::CreateHolder();


    // auto group = Metric<Counter<double>>("my counter", "some help");

    // Holder::Register(group);







    return 0;
}
