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
/**
 * @brief Class represent a Histigram metric.
 *
 * passing an empty vector to a pair for inizialization Counter<T>, because emplace() require values for Counter`s Ctor.
 *
 * @param T represent metrcic type value, double or int64 ONLY! default is int64.
 * @param label_values Value of Labels, for example "method = get", "get" is label value. You may specify or not.
 *        Amount of label values MUST be the same as label names specifued in Metric.      
 * @param vector of bounds(buckets), each bucket has type T.      
 */
template<class T = double>
class Histogram : public IBaseMetric
{
    public:
        Histogram(std::vector<std::string>&& label_values = {}): IBaseMetric(std::move(label_values)), m_gauge_summ()
        {
            m_bound_counter.emplace(std::make_pair(std::numeric_limits<T>::infinity(), std::vector<std::string>{}));
        }

        void Buckets(const std::vector<T>& bounds)
        {
            for(size_t i = 0; i < bounds.size(); ++i)
            {
                m_bound_counter.emplace(std::make_pair(bounds[i], std::vector<std::string>{}));
            }
            m_iterator = m_bound_counter.begin();
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
            m_iterator = m_bound_counter.begin();
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
            m_iterator = m_bound_counter.begin();
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
            std::cout << "ITERATOR: " << m_iterator->first << std::endl;
            m_iterator++;
            return std::make_pair(std::to_string(m_iterator->first), m_iterator->second.GetValueAsString().first); 
            // return std::make_pair("0.0", "7.9"); 
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

        void Size()
        {
            LOG(m_bound_counter.size());
        }

    private:
        Gauge<T> m_gauge_summ;
        std::map<T, Counter<T>> m_bound_counter{};
        typename std::map<T, Counter<T>>::iterator m_iterator{};
};
