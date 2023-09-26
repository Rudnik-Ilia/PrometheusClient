#pragma once

#include <map>
#include <utility>  
#include <limits>

#include "init.hpp"
#include "gauge.hpp"
#include "../include/counter.hpp"
#include "../include/anxilary_functions.hpp"

template <class T>
using BucketsLimit = std::vector<T>;

template<class T = double>
class Histogram
{
    public:
        Histogram(std::vector<T> bounds, std::vector<std::string> label_values = {}): m_label_values(std::move(label_values)), m_gauge_summ()
        {
            for(size_t i = 0; i < bounds.size(); ++i)
            {
                m_bound_counter.emplace(std::make_pair(bounds[i], std::vector<std::string>{}));
            }
            m_bound_counter.emplace(std::make_pair(std::numeric_limits<T>::infinity(), std::vector<std::string>{}));
        }

        void Default()
        {

        }

        void Linear(T start, T step, uint64_t count)
        {

        }

        void Exponential(T start, T factor, uint64_t count)
        {

        }

        void Observe(T value)
        {
            auto iter = m_bound_counter.lower_bound(value);
            if(iter != m_bound_counter.end())
            {
                while (iter != m_bound_counter.end())
                {
                    iter->second.Inc();
                    ++iter;
                }
            }
            m_gauge_summ.Inc(value);
        }

        void Show()
        {
            for(auto &iter : m_bound_counter)
            {
                LOG(iter.second.GetValue());
            }
            LOG(m_gauge_summ.GetValue());
        }

        // double _summ()
        // {

        // }

        // double _count()
        // {

        // }

    private:
        std::vector<std::string> m_label_values;
        Gauge<T> m_gauge_summ;
        std::map<T, Counter<T>> m_bound_counter{};
};
