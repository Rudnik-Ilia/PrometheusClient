#pragma once

#include "init.hpp"
#include "anxilary_functions.hpp"
#include "metric.hpp"
#include "gauge.hpp"
#include "histogram.hpp"

class Storage
{
    public:
        static Storage& CreateStorage()
        {
            static Storage storage;
            return storage;
        }

        template <template <typename> class MetricType, class T = int64_t>
        void AddToStorage(std::shared_ptr<MetricType<T>>& metrica)
        {
          
        }

    private:
        Storage() = default;

        template <template <typename> class MetricType, class T = int64_t>
        static std::vector<std::shared_ptr<MetricType<T>>> m_all_metrics;
};

template <template <typename> class MetricType, class T>
std::vector<std::shared_ptr<MetricType<T>>> Storage::m_all_metrics{};

// ***************************************************************************************************************

template <template <typename> class Type, class T = int64_t>
class prometheus
{
    public:

        static std::shared_ptr<Metric<Type<T>>> Base()
        {
            auto smart_ptr = std::make_shared<Metric<Type<T>>>();
            m_all_metrics.push_back(smart_ptr);
            return smart_ptr;
        }

        static std::shared_ptr<Metric<Type<T>>> Base(std::string name, std::string help, std::vector<std::string> label_names = {})
        {
            auto smart_ptr = std::make_shared<Metric<Type<T>>>(std::move(name), std::move(help) , std::move(label_names));
            m_all_metrics.push_back(smart_ptr);
            return smart_ptr;
        }

        static std::shared_ptr<Type<T>> Make(std::string name, std::string help, std::vector<std::string> label_names = {}, std::vector<std::string> label_values = {})
        {
            auto metric_wrapper = std::make_shared<Metric<Type<T>>>(std::move(name), std::move(help) , std::move(label_names));
            m_all_metrics.push_back(metric_wrapper);
            auto metris_entity = metric_wrapper->Build(std::move(label_values));
            return metris_entity;
        }

        static void Register(Storage& storage)
        {
            
        }

        static void Storage()
        {
            for(auto& iter : m_all_metrics)
            {
                iter->Show();
            }
        }

    private:
        // Storage m_storage;
        static std::vector<std::shared_ptr<Metric<Type<T>>>> m_all_metrics;
};

template <template <typename> class Type, class T>
std::vector<std::shared_ptr<Metric<Type<T>>>> prometheus<Type,T>::m_all_metrics{};


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