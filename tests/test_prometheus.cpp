#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>

#include "../include/init.hpp"d
#include "../include/prometheus.hpp"

using vectorStr = std::vector<std::string>;
template<class T>
using Buckets = std::vector<T>;

int main()
{
    auto c1 = prometheus<Counter, int64_t>::Make("name_counter", "help_counter", {"method"}, {"get"});
    c1->Inc();
    LOG(c1->GetValue());

    auto g1 = prometheus<Gauge, int64_t>::Make("name_gauge", "help_gauge", {"CPU"}, {"idle"});
    g1->Inc(9);

    LOG(g1->GetValue());
    {
        auto g = g1->Track<NANOSEC>();
        usleep(3456567);
    }
    LOG(g1->GetValue());


    // auto c1 = prometheus<Counter, int64_t>::Make("name", "help_counter");
    // c1->Inc();
    // c1->Inc();
    // c1->Inc();
    // LOG(c1->GetValue());

    // auto c2 = prometheus<Counter, double>::Make("name", "help_counter", {"method"}, {"get"});
    // auto c3 = prometheus<Counter, double>::Make("name", "help_counter", {"method"}, {"post"});
    // c2->Inc();
    // c2->Inc(2.2);
    // c2->Inc(0.3);
    // LOG(c2->GetValue());

    // auto g1 = prometheus<Gauge, double>::Make("name_gauge", "help_gauge", {"CPU"}, {"idle"});
    // g1->Inc(3);
    // g1->Dec(1);
    // g1->Inc(10);
    // LOG(g1->GetValue());

    // {
    //     auto tmp = g1->Track<>();
    //     usleep(3456567);
    // }
    // LOG(g1->GetValue());

    // auto h1 = prometheus<Histogram, double>::Make("histo", "help_histo", {"latency"}, {"request"}, {1.0, 2.0, 3.0});

    // h1->Observe(0.5);
    // h1->Observe(1.5);
    // h1->Observe(2.5);
    // h1->Observe(2.5);
    // h1->Observe(2.5);
    // h1->Observe(3.9);
    // h1->Observe(3.9);
    // h1->Observe(0.1);

    // h1->Show();
    // h1->Reset();
    // h1->Show();
    
    // auto h2 = prometheus<Histogram>::Make("histo", "help_histo", {"latency"});

    return 0;
}