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
    auto c1 = Metric<Counter<int64_t>>("counter", "help",{"method"}).Build(vectorStr{"get"});
    auto c2 = Metric<Counter<int64_t>>("counter", "help",{"method"}).Build(vectorStr{"post"});
    
    c1->Inc();
    c1->Inc();
    c1->Inc();
    LOG(c1->GetValue());

    auto g1 = Metric<Gauge<int64_t>>("gauge", "help_gauge", {"CPU"}).Build(vectorStr{"idle"});
    g1->Inc(3);
    g1->Dec(1);
    g1->Inc(10);
    LOG(g1->GetValue());

    {
        auto tmp = g1->Track<SECOND>();
        usleep(3456567);
    }

    LOG(g1->GetValue());

    auto h1 = Metric<Histogram<double>>("histo", "help_histo", {"latency"}).Build(vectorStr{}, Buckets<double>{1.0, 2.0, 3.0});

    h1->Observe(0.5);
    h1->Observe(1.5);
    h1->Observe(2.5);
    h1->Observe(2.5);
    h1->Observe(2.5);
    h1->Observe(3.9);
    h1->Observe(3.9);
    h1->Observe(0.1);

    h1->Show();
    h1->Reset();
    h1->Show();

    return 0;
}