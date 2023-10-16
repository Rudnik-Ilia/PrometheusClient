#pragma once

#include "init.hpp"
#include "metric.hpp"

class Holder
{
    public:
        static Holder& CreateHolder()
        {
            static Holder holder;
            return holder;
        }

        template <template <typename> class MetricType, class T = int64_t>
        static void Register(MetricType<T>& metric)
        {
            auto sm_ptr = std::make_unique<MetricType<T>>(metric);
            m_all_metrics<MetricType<T>>.push_back(sm_ptr);
        }

    private:
        Holder() = default;

        template <template <typename> class MetricType, class T = int64_t>
        static std::vector<std::unique_ptr<MetricType<T>>> m_all_metrics;
};

template <template <typename> class MetricType, class T>
std::vector<std::unique_ptr<MetricType<T>>> Holder::m_all_metrics{};
