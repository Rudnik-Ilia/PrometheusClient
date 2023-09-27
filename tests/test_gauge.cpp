#include <unistd.h>

#include "../include/gauge.hpp"
#include "../include/anxilary_functions.hpp"


// g++ -std=c++17 test_counter.cpp -lpthread
// g++ -std=c++17 test_gauge.cpp ../src/anxilary_functions.cpp  -lpthread

using vectorStr = std::vector<std::string>;
auto g1 = std::make_shared<Gauge<double>>(vectorStr{"get", "post"});

void Now()
{
    const auto time = std::time(nullptr);
    LOG(static_cast<double>(time));
    LOG(static_cast<int64_t>(time));
}

int main()
{
    auto g_dur = std::make_shared<Gauge<int64_t, NANOSEC>>(vectorStr{});
    LOG(g1->GetValue());

    {
        auto t = g_dur->Track();
        sleep(1);
    }
    LOG(g_dur->GetValue());
    {
        auto t = g_dur->Track();
        sleep(1);
    }
    LOG(g_dur->GetValue());

    g1->Set(3.33);
    g1->Inc(3.33);
    g1->Dec(1.45);

    g1->SetTimeNow();
    g_dur->SetTimeNow();
    Now();
    LOG("------------------------------------------------------");
    LOG(g1->GetValue());
    LOG(g_dur->GetValue());



    return 0;
}