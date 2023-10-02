#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>

#include "../include/init.hpp"
#include "../include/prometheus.hpp"

using vectorStr = std::vector<std::string>;
template<class T>
using Buckets = std::vector<T>;


int main()
{
    auto metr = prometheus<Counter, int64_t>::Base();

    metr->Name("counter").Help("some help").Labels({"get"});
    LOG(metr->GetLabels()[0]);
    metr->Show();
    

    return 0;
}