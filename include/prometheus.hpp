#pragma once

#include "init.hpp"
#include "anxilary_functions.hpp"
#include "metric.hpp"
#include "gauge.hpp"
#include "histogram.hpp"

template <class T = int64_t, class U = NANOSEC>
class prometheus
{
    public:
        static std::shared_ptr<Counter<T>> MakeCoumter(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {})
        {
            return Metric<Counter<T>>(std::move(name), std::move(help) , std::move(label_names)).Build(std::move(label_values));
        }
        
        static std::shared_ptr<Gauge<T, U>> MakeGauge(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {})
        {
            return Metric<Gauge<T, U>>(std::move(name), std::move(help) , std::move(label_names)).Build(std::move(label_values));
        }

        static std::shared_ptr<Histogram<T>> MakeHistogram(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {}, std::vector<T> buckets = {})
        {
            return Metric<Histogram<T>>(std::move(name), std::move(help) , std::move(label_names)).Build(std::move(label_values), std::move(buckets));
        }

    private:

        std::vector<std::unique_ptr<Metric<T>> m_all_metrics;
};