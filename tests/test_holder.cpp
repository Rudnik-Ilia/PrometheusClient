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

    // auto c_1 = prometheus<Counter, int64_t>::Make("name_counter_1", "help with c1", {"method"},{"post"});

    auto Family_1 = prometheus<Counter, int64_t>::Base("count_from_family_1", "help fam_1", {"method", "errors"});

    auto c_2 = Family_1->AddValues({"get", "simple"});
    auto c_3 = Family_1->AddValues({"post", "hard"});

    c_2->Inc(111);
    c_3->Inc(88);

    // auto Family_2 = prometheus<Gauge, double>::Base("name_gauge", "help2", {"CPU"});
    // auto g_1 = Family_2->AddValues({"idle"});





    LOG(holder->GetSize());

    LOG(Family_1->GetTypeAsString());
    // LOG(Family_2->GetTypeAsString());

    LOG("****************************************");

    holder->Bridge();




    return 0;
}
