#pragma once

#include "init.hpp"
#include "anxilary_functions.hpp"
#include "metric.hpp"
#include "gauge.hpp"
#include "histogram.hpp"


// ***************************************************************************************************************

template <template <typename> class Type, class T = int64_t>
class prometheus
{
    public:
        static std::shared_ptr<Metric<Type<T>>> Base(std::string name, std::string help, std::vector<std::string> label_names = {})
        {
            auto smart_ptr = std::make_shared<Metric<Type<T>>>(std::move(name), std::move(help) , std::move(label_names));
            return smart_ptr;
        }

        static std::shared_ptr<Type<T>> Make(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {})
        {
            auto metric_wrapper = std::make_shared<Metric<Type<T>>>(std::move(name), std::move(help) , std::move(label_names));
            auto metris_entity = metric_wrapper->Build(std::move(label_values));
            return metris_entity;
        }
};
// OLD Ctor without param

        // static std::shared_ptr<Metric<Type<T>>> Base()
        // {
        //     auto smart_ptr = std::make_shared<Metric<Type<T>>>();
        //     return smart_ptr;
        // }


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