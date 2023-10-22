#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>
#include <typeinfo>

#include "../include/init.hpp"
#include "../include/prometheus.hpp"
#include "../include/holder.hpp"
#include "../include/singleton.hpp"



int main()
{
    auto holder = Singleton<Holder>::GetInstance();

    auto c_1 = prometheus<Counter, int64_t>::Make("counter 1", "help", {"method"},{"post"});

    auto Family_1 = prometheus<Counter, int64_t>::Base("count", "help", {"method"});
    auto Family_2 = prometheus<Gauge, double>::Base("gauge", "help2", {"CPU"});





    LOG(holder->GetSize());

    LOG((MetricType::COUNTER < MetricType::HISTOGRAM));

    LOG(Family_1->GetTypeAsString());
    LOG(Family_2->GetTypeAsString());




    return 0;
}
