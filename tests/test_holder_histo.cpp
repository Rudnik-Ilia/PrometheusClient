#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>
#include <typeinfo>

#include "../include/prometheus.hpp"
#include "../include/counter.hpp"

// g++ --std=c++17 test_holder_histo.cpp ../src/holder.cpp ../src/anxilary_functions.cpp 

const std::vector<double> test_buckets_double{0.5, 1.0, 1.5, 2.0, 2.5, 2.75};
const std::vector<int64_t> test_buckets_64{10, 20, 30, 40, 50, 65};

const double TEST_ARR_DOUBLE[] = {0.1, 0.2, 1.7, 2.3, 2.2, 3.0, 0.7, 2.1, 2.74, 2.75};
const int64_t TEST_ARR_64_1[] = {1, 0, 17, 23, 22, 30, 71, 66, 274, 54};
const int64_t TEST_ARR_64_2[] = {34, 4, 44, 22, 22, 23, 7, 6, 2, 5};

int main()
{
    auto holder = Singleton<Holder>::GetInstance();

    auto group_histo_double = prometheus<Histogram, double>::GetGroup("First_group", "from family from vector double", {"delay", "data"});
    auto g_1 = group_histo_double->AddValues({"DB", "10.23.1986"});
    g_1->Buckets(test_buckets_double);

    auto group_histo_64 = prometheus<Histogram, int64_t>::GetGroup("Second_group", "from family from vector int64", {"amount_request", "data"});

    auto g_2 = group_histo_64->AddValues({"GET", "10.01.2023"});
    g_2->Buckets(test_buckets_64);

    auto g_3 = group_histo_64->AddValues({"POST", "03.12.1492"});
    g_3->Buckets(test_buckets_64);

    for(size_t i = 0; i < 10; ++i)
    {
        g_1->Observe(TEST_ARR_DOUBLE[i]);
        g_2->Observe(TEST_ARR_64_1[i]);
        g_3->Observe(TEST_ARR_64_2[i]);
    }

    LOG(holder->CollectData());
    LOG("************************************************************************");

    return 0;
}
