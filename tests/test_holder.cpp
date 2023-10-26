#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>
#include <typeinfo>

#include "../include/prometheus.hpp"

#include "../include/counter.hpp"


// g++ --std=c++17 test_holder.cpp ../src/holder.cpp ../src/anxilary_functions.cpp 

int main()
{
    auto holder = Singleton<Holder>::GetInstance();

    auto c_1 = prometheus<Counter, int64_t>::GetMetric("base_counter", "every request", {"HTTP"},{"all"});

    auto Family_1 = prometheus<Counter, int64_t>::GetGroup("count_from_family_1", "help fam_1", {"method", "errors", "id"});

    auto c_2 = Family_1->AddValues({"get", "simple", "345"});
    auto c_3 = Family_1->AddValues({"post", "hard", "001"});

    c_2->Inc(111);
    c_3->Inc(88);

    auto Family_2 = prometheus<Gauge, int64_t>::GetGroup("name_gauge", "help2", {"CPU"});
    auto g_1 = Family_2->AddValues({"idle"});

    {
        auto gg = g_1->Track<MICROSEC>();
        sleep(1);
    }

    LOG(holder->GetSize());

    LOG(Family_1->GetTypeAsString());
    LOG(Family_2->GetTypeAsString());

    LOG("************************************************************************");
    LOG(holder->CollectData());

    g_1->SetTimeNow();

    LOG("************************************************************************");
    LOG(holder->CollectData());

    // // auto h_1 = prometheus<Histogram, double>::MakeMetric("histo_latency", "help_histo", {"request", "CPU"}, {"get", "idle"});
    auto group_histo = prometheus<Histogram, double>::GetGroup("timeout", "measure_timeout", {"client"});

    auto h_2 = group_histo->AddValues({"DB"});
    h_2->Buckets({111, 222, 333});
    

    // h_1->Buckets({1.0, 2.0, 3.0});
    // h_1->LinearBuckets(1.0, 1, 3);
    // h_1->Observe(0);
    // h_1->Observe(1.2);

    LOG("************************************************************************");
    LOG(holder->CollectData());
    LOG("************************************************************************");

    // h_2->Show();

    // h_1->Size();

    return 0;
}
