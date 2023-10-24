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

    auto c_1 = prometheus<Counter, int64_t>::Make("base_counter", "every request", {"HTTP"},{"all"});

    auto Family_1 = prometheus<Counter, int64_t>::Base("count_from_family_1", "help fam_1", {"method", "errors", "id"});

    auto c_2 = Family_1->AddValues({"get", "simple", "345"});
    auto c_3 = Family_1->AddValues({"post", "hard", "001"});

    c_2->Inc(111);
    c_3->Inc(88);

    auto Family_2 = prometheus<Gauge, int64_t>::Base("name_gauge", "help2", {"CPU"});
    auto g_1 = Family_2->AddValues({"idle"});

    {
        auto gg = g_1->Track<MICROSEC>();
        sleep(3);
    }

    LOG(holder->GetSize());

    LOG(Family_1->GetTypeAsString());
    // LOG(Family_2->GetTypeAsString());

    LOG("****************************************");

    // holder->Bridge();
    LOG(holder->GetData());




    return 0;
}
