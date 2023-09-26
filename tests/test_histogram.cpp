#include <iostream>
#include <map>
#include <memory>

#include "../include/histogram.hpp"

using vectorStr = std::vector<std::string>;

template<class T>
using vectorVal = std::vector<T>;

int main()
{
    auto h1 = std::make_shared<Histogram<>>(std::vector<double>{1.0, 2.0, 3.0}, vectorStr{"duration"});

    h1->Observe(0.5);
    h1->Observe(1.5);
    h1->Observe(2.5);
    h1->Observe(2.5);
    h1->Observe(2.5);
    h1->Observe(3.9);
    h1->Observe(3.9);


    h1->Show();

    return 0;
}
