#pragma once

#include "group.hpp"
#include "holder.hpp"
#include "singleton.hpp"

template <template <typename> class Type, class T = int64_t>
class prometheus
{
    public:
        static std::shared_ptr<Group<Type<T>>> GetGroup(std::string name, std::string help, std::vector<std::string> label_names = {})
        {
            auto group_ptr = std::make_shared<Group<Type<T>>>(std::move(name), std::move(help) , std::move(label_names));

            Singleton<Holder>::GetInstance()->RegistrateMetric(group_ptr);
            
            return group_ptr;
        }

        static std::shared_ptr<Type<T>> GetMetric(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {})
        {
            auto group_ptr = std::make_shared<Group<Type<T>>>(std::move(name), std::move(help) , std::move(label_names));

            Singleton<Holder>::GetInstance()->RegistrateMetric(group_ptr);

            auto metric = group_ptr->Build(std::move(label_values));
            
            return metric;
        }
};



// OLD factory methods

        // static std::shared_ptr<Counter<T>> MakeCoumter(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {})
        // {
        //     auto smart_ptr = Metric<Counter<T>>(std::move(name), std::move(help) , std::move(label_names)).Build(std::move(label_values));
        //     return smart_ptr;
        // }
        
        // static std::shared_ptr<Gauge<T, U>> MakeGauge(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {})
        // {
        //     auto smart_ptr = Metric<Gauge<T, U>>(std::move(name), std::move(help) , std::move(label_names)).Build(std::move(label_values));
        //     return smart_ptr;
        // }

        // static std::shared_ptr<Histogram<T>> MakeHistogram(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {}, std::vector<T> buckets = {})
        // {
        //     return Metric<Histogram<T>>(std::move(name), std::move(help) , std::move(label_names)).Build(std::move(label_values), std::move(buckets));
        // }
        // static Metric<Counter<T>> Base(std::string name, std::string help, std::vector<std::string> label_names = {})
        // {
        //     auto m = Metric<Counter<T>>(std::move(name), std::move(help) , std::move(label_names));
        //     return m;
        // }