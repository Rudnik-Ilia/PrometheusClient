#include <iostream>
#include <map>
#include <memory>

#include "../include/histogram.hpp"

using vectorStr = std::vector<std::string>;

template<class T>
using Buckets = std::vector<T>;

int main()
{
    // auto h1 = std::make_shared<Histogram<>>(vectorStr{"duration"});
    // h1->Exponential(1.0, 3.0, 3);
    // auto h1 = std::make_shared<Histogram<>>(vectorStr{"duration"});
    // h1->Linear(1.0, 1.0, 3);
    auto h1 = std::make_shared<Histogram<>>(vectorStr{"duration"}, Buckets<double>{1.0, 2.0, 3.0});
    // auto h1 = std::make_shared<Histogram<>>(Buckets<double>{1.0, 2.0, 3.0});

    h1->Observe(0.5);
    h1->Observe(1.5);
    h1->Observe(2.5);
    h1->Observe(2.5);
    h1->Observe(2.5);
    h1->Observe(3.9);
    h1->Observe(3.9);
    h1->Observe(0.1);


    h1->Show();
    h1->Reset();
    h1->Show();

    return 0;
}
