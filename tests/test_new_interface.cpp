#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>

#include "../include/init.hpp"
#include "../include/prometheus.hpp"

using vectorStr = std::vector<std::string>;
template<class T>
using Buckets = std::vector<T>;


template <typename T>
std::vector<Metric<T>> vec{};

int main()
{
    auto metr = prometheus<Counter, int64_t>::Base();

    auto metr2 = prometheus<Gauge, int64_t>::Base();

    auto c1 = metr->Name("counter").Help("some help").Labels({"method"}).Build(vectorStr{"get"});
    c1->Inc();
    LOG(metr->GetLabels()[0]);
    metr->Show();

    // auto c1 = prometheus<Counter, double>::Make("name_counter", "help_counter", {"method"}, {"get"});




    return 0;
}