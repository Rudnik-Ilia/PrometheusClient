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
class Histogram : public IBaseMetric
{
    public:
        Histogram(std::vector<std::string>&& label_values = {}, std::vector<T> bounds = {}): IBaseMetric(std::move(label_values)), m_gauge_summ()
        {
            for(size_t i = 0; i < bounds.size(); ++i)
            {
                m_bound_counter.emplace(std::make_pair(bounds[i], std::vector<std::string>{}));
            }
            m_bound_counter.emplace(std::make_pair(std::numeric_limits<T>::infinity(), std::vector<std::string>{}));
            // m_iterator = m_label_values.begin();
        }

        void LinearBuckets(T start, T step, uint64_t count)
        {            
            T value_to_insert = start;
            m_bound_counter.emplace(std::make_pair(value_to_insert, std::vector<std::string>{}));
            
            for(size_t i = 0; i < count - 1; ++i)
            {
                value_to_insert += step;
                m_bound_counter.emplace(std::make_pair(value_to_insert, std::vector<std::string>{}));
            }
        }

        void ExponentialBuckets(T start, T factor, uint64_t count)
        {
            T value_to_insert = start;
            m_bound_counter.emplace(std::make_pair(value_to_insert, std::vector<std::string>{}));

            for(size_t i = 0; i < count - 1; ++i)
            {
                value_to_insert *= factor;
                m_bound_counter.emplace(std::make_pair(value_to_insert, std::vector<std::string>{}));
            }
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

        void Reset()
        {
            for(auto& iter : m_bound_counter)
            {
                iter.second.Reset();
            }
        }

        std::pair<std::string, std::string> GetValueAsString()
        {
            // m_iterator = m_bound_counter.begin();

            // ++m_iterator;
            // return std::make_pair(std::to_string(m_iterator->first), m_iterator->second.GetValueAsString().first); 
            return std::make_pair("0.0", "7.9"); 
        }
        
        // FOR TESTING
        void Show()
        {
            for(auto &iter : m_bound_counter)
            {
                LOG("bucket: ");
                LOG(iter.first);
                LOG(iter.second.GetValue());
            }
            LOG("_summ: ");
            LOG(m_gauge_summ.GetValue());
        }

    private:
        Gauge<T> m_gauge_summ;
        std::map<T, Counter<T>> m_bound_counter{};
        // typename std::map<T, Counter<T>>::iterator m_iterator{};
};
