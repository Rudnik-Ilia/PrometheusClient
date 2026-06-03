#pragma once

#include <map>
#include <utility>
#include <limits>

#include "outScope.hpp"

template <class T>
using BucketsLimit = std::vector<T>;

/**
 * @brief Histogram metric with configurable buckets.
 * @tparam T Value type: double or int64_t (default: double)
 */
template<class T = double, typename = std::enable_if_t<std::is_same_v<T, double> || std::is_same_v<T, int64_t>>>
class Histogram : public IBaseMetric
{
public:
    explicit Histogram(std::vector<std::string>&& label_values = {})
        : IBaseMetric(std::move(label_values))
        , m_gauge_summ()
    {
        m_bounds_counters.emplace(std::make_pair(std::numeric_limits<T>::infinity(), std::vector<std::string>{}));
    }

    void Buckets(const std::vector<T>& bounds)
    {
        for (const auto& bound : bounds)
        {
            m_bounds_counters.emplace(std::make_pair(bound, std::vector<std::string>{}));
        }
        ResetIterator();
    }

    void LinearBuckets(T start, T step, uint64_t count)
    {
        T value = start;
        m_bounds_counters.emplace(std::make_pair(value, std::vector<std::string>{}));

        for (size_t i = 0; i < count - 1; ++i)
        {
            value += step;
            m_bounds_counters.emplace(std::make_pair(value, std::vector<std::string>{}));
        }
        ResetIterator();
    }

    void ExponentialBuckets(T start, T factor, uint64_t count)
    {
        T value = start;
        m_bounds_counters.emplace(std::make_pair(value, std::vector<std::string>{}));

        for (size_t i = 0; i < count - 1; ++i)
        {
            value *= factor;
            m_bounds_counters.emplace(std::make_pair(value, std::vector<std::string>{}));
        }
        ResetIterator();
    }

    void Observe(T value)
    {
        auto iter = m_bounds_counters.lower_bound(value);
        while (iter != m_bounds_counters.end())
        {
            iter->second.Inc();
            ++iter;
        }
        m_gauge_summ.Inc(value);
    }

    void Reset()
    {
        for (auto& [bound, counter] : m_bounds_counters)
        {
            counter.Reset();
        }
    }

    std::pair<std::string, std::string> GetValueAsString() override
    {
        std::pair<std::string, std::string> bucket_value(
            std::to_string(m_iterator->first),
            m_iterator->second.GetValueAsString().first);

        if (++m_iterator == m_bounds_counters.end())
        {
            --m_iterator;
            bucket_value = std::make_pair(
                std::to_string(m_gauge_summ.GetValue()),
                m_iterator->second.GetValueAsString().first);
            ResetIterator();
        }
        return bucket_value;
    }

    int64_t GetSize() const override
    {
        return static_cast<int64_t>(m_bounds_counters.size());
    }

    T GetValue() const
    {
        return m_gauge_summ.GetValue();
    }

private:
    void ResetIterator()
    {
        m_iterator = m_bounds_counters.begin();
    }

    Gauge<T> m_gauge_summ;
    std::map<T, Counter<T>> m_bounds_counters{};
    mutable typename std::map<T, Counter<T>>::iterator m_iterator{};
};
