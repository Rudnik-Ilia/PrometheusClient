#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>

#include "../include/init.hpp"
#include "../include/prometheus.hpp"

using vectorStr = std::vector<std::string>;
template<class T>
using Buckets = std::vector<T>;


int main()
{
    std::vector<std::shared_ptr<IMetric>> storage{};

    auto Family_1 = prometheus<Counter, int64_t>::Base("count", "help", {"method"});
    auto Family_2 = prometheus<Gauge, double>::Base("gauge", "help2", {"CPU"});
    auto Family_Error = prometheus<Counter, int64_t>::Base("mistake count", "all mistake",{"tt"});

    storage.push_back(Family_1);
    storage.push_back(Family_2);
    storage.push_back(Family_Error);

    auto error = Family_Error->AddValues({"ANY"});
    error->Inc(123456);

    auto c_3 = Family_1->AddValues({"post"});
    auto c_4 = Family_1->AddValues({"get"});
    auto c_5 = Family_1->AddValues({"patch"});

    c_3->Inc(35);
    LOG(c_3->GetValue());
    c_4->Inc(177);
    LOG(c_4->GetValue());
    c_5->Inc(4556);
    LOG(c_5->GetValue());


    LOG((Family_1->GetType() == MetricType::COUNTER));
    LOG((Family_2->GetType() == MetricType::GAUGE));


    // Family_1->Show();
    // Family_2->Show();
    // Family_Error->Show();



    return 0;
}