#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>

#include "../include/init.hpp"
#include "../include/metric.hpp"
#include "../include/gauge.hpp"
#include "../include/histogram.hpp"

using vectorStr = std::vector<std::string>;
template<class T>
using Buckets = std::vector<T>;


int main()
{

    auto M1 = Metric<Counter<int64_t>>("name_count", "help", {"method"});
    auto M2 = Metric<Counter<int64_t>>("name_count_2", "help_2", {"method_2", "some"});
    auto c1 = M1.Build(vectorStr{"get"});
    auto c2 = M1.Build(vectorStr{"post"});

    M2.Build(vectorStr{"second"}).get()->Inc();

    c1->Inc();
    c1->Inc();

    M1.Show();
    M2.Show();

    return 0;
}