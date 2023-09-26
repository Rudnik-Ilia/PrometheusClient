#include <unistd.h>

#include "../include/gauge.hpp"
#include "../include/anxilary_functions.hpp"


// g++ -std=c++17 test_counter.cpp -lpthread
// g++ -std=c++17 test_gauge.cpp ../src/anxilary_functions.cpp  -lpthread

using vectorStr = std::vector<std::string>;
auto g1 = std::make_shared<Gauge<double>>(vectorStr{"get", "post"});



int main()
{
    auto g_dur = std::make_shared<Gauge<int64_t, MILLISEC>>(vectorStr{"get_duration"});
    LOG(g1->GetValue());

    {
        auto t = g_dur->Duration();
        sleep(3);
    }
    LOG(g_dur->GetValue());
    {
        auto t = g_dur->Duration();
        sleep(5);
    }
    LOG(g_dur->GetValue());

    g1->Set(3.33);
    g1->Inc(3.33);
    g1->Dec(1.45);
    LOG(g1->GetValue());



    return 0;
}