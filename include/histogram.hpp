#pragma once

#include <map>
#include <utility>  
#include <limits>

#include "outScope.hpp"

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
            m_bounds_counters.emplace(std::make_pair(std::numeric_limits<T>::infinity(), std::vector<std::string>{}));
        }

        void Buckets(const std::vector<T>& bounds)
        {
            for(size_t i = 0; i < bounds.size(); ++i)
            {
                m_bounds_counters.emplace(std::make_pair(bounds[i], std::vector<std::string>{}));
            }
            m_iterator = m_bounds_counters.begin();
        }

        void LinearBuckets(T start, T step, uint64_t count)
        {            
            T value_to_insert = start;
            m_bounds_counters.emplace(std::make_pair(value_to_insert, std::vector<std::string>{}));
            
            for(size_t i = 0; i < count - 1; ++i)
            {
                value_to_insert += step;
                m_bounds_counters.emplace(std::make_pair(value_to_insert, std::vector<std::string>{}));
            }
            m_iterator = m_bounds_counters.begin();
        }

        void ExponentialBuckets(T start, T factor, uint64_t count)
        {
            T value_to_insert = start;
            m_bounds_counters.emplace(std::make_pair(value_to_insert, std::vector<std::string>{}));

            for(size_t i = 0; i < count - 1; ++i)
            {
                value_to_insert *= factor;
                m_bounds_counters.emplace(std::make_pair(value_to_insert, std::vector<std::string>{}));
            }
            m_iterator = m_bounds_counters.begin();
        }

        void Observe(T value)
        {
            auto iter = m_bounds_counters.lower_bound(value);
            if(iter != m_bounds_counters.end())
            {
                while (iter != m_bounds_counters.end())
                {
                    iter->second.Inc();
                    ++iter;
                }
            }
            m_gauge_summ.Inc(value);
        }

        void Reset()
        {
            for(auto& iter : m_bounds_counters)
            {
                iter.second.Reset();
            }
        }

        std::pair<std::string, std::string> GetValueAsString() override
        {
            std::pair<std::string, std::string> bucket_value({std::to_string(m_iterator->first), m_iterator->second.GetValueAsString().first}); 
            if(++m_iterator == m_bounds_counters.end())
            {
                LOG("END");
                // bucket_value = std::make_pair();
                m_iterator = m_bounds_counters.begin();
            }
            return bucket_value;
        }
        
        int64_t GetSize() const override
        {
            return m_bounds_counters.size();
        }

        // FOR TESTING
        void Show()
        {
            for(auto &iter : m_bounds_counters)
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
        std::map<T, Counter<T>> m_bounds_counters{};
        mutable typename std::map<T, Counter<T>>::iterator m_iterator{};
};
