#include <iostream>
#include <unistd.h>

#include "include/prometheus.hpp"

std::vector<double> test_buckets_1{0.5, 1.0, 1.5, 2.0};

int main()
{
    auto holder = Singleton<Holder>::GetInstance();

    auto c_1 = prometheus<Counter, double>::GetMetric("base_counter", "every request", {"HTTP"},{"all"});

    auto Family_1 = prometheus<Counter, int64_t>::GetGroup("count_from_family_1", "help fam_1", {"method", "errors", "id"});

    auto c_2 = Family_1->AddValues({"get", "simple", "345"});
    auto c_3 = Family_1->AddValues({"post", "hard", "001"});

    c_2->Inc(111);
    c_3->Inc(88);

    auto Family_2 = prometheus<Gauge, int64_t>::GetGroup("name_gauge", "help2", {"CPU"});
    auto g_1 = Family_2->AddValues({"idle"});

    {
        auto gg = g_1->Track<NANOSEC>();
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

    auto group_histo = prometheus<Histogram, double>::GetGroup("timeout", "measure_timeout", {"client"});

    auto h_2 = group_histo->AddValues({"DB"});

    h_2->Buckets(test_buckets_1);
    h_2->Observe(0.1);
    h_2->Observe(1.7);
    h_2->Observe(0.6);
    h_2->Observe(2.22);

    LOG("************************************************************************");
    LOG(holder->CollectData());
    LOG("************************************************************************");
    LOG(holder->CollectData());
    return 0;
}