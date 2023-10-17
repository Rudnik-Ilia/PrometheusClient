#include <thread>
#include <mutex>

#include "../include/counter.hpp"
#include "../include/anxilary_functions.hpp"
// g++ -std=c++17 test_counter.cpp -lpthread

using vectorStr = std::vector<std::string>;
std::mutex counterMutex{};

int64_t COUNTER = 0;
auto c1 = std::make_shared<Counter<int64_t>>(vectorStr{"GET"});
auto c2 = std::make_shared<Counter<double>>(vectorStr{"POST"});


void WorkTread(std::shared_ptr<Counter<int64_t>>& c1)
{
    for(size_t i = 0; i < 1000000; ++i)
    {
        c1->Inc();
    }
}

void WorkTread_1()
{
    for(size_t i = 0; i < 1000000; ++i)
    {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++COUNTER;
    }
}

int main()
{
    Counter<double> c3;


    LOG(c1->GetValue());
    // LOG(COUNTER);

    std::thread pool[2];

    for(size_t i = 0; i < 2; ++i)
    {
        pool[i] = std::thread(&WorkTread, std::ref(c1));
    }

    for(size_t i = 0; i < 2; ++i)
    {
        pool[i].join();
    }

    LOG(c1->GetValue());
    // LOG(COUNTER);

    LOG(c2->GetValue());
    c2->Inc(3.33);
    c1->Inc(3.33);
    LOG(c2->GetValue());
    LOG(c1->GetValue());
    c1->Reset();

    LOG(c1->GetLabels()[0]);
    LOG(c2->GetLabels()[0]);

    
    return 0;
}
