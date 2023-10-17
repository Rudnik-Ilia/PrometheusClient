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
    auto c2 = prometheus<Counter, double>::Make("name", "help_counter", {"method"}, {"get"});
    auto c3 = prometheus<Counter, int64_t>::Make("name", "help_counter", {"method"}, {"post"});
    auto c4 = prometheus<Counter, int64_t>::Base();
    auto g2 = prometheus<Gauge, int64_t>::Base();


    auto g1 = prometheus<Gauge, double>::Make("name_gauge", "help_gauge", {"CPU"}, {"idle"});

    LOG(typeid(c2).name());
    LOG(typeid(c3).name());
    LOG(typeid(g1).name());

    LOG(typeid(g2).name());
    LOG(typeid(c4).name());






    return 0;
}
