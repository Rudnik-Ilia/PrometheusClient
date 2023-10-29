#pragma once

#include <type_traits>

#include "anxilary_functions.hpp"
#include "IBaseMetric.hpp"

/**
 * @brief Class represent a counter.
 *
 * You can only increase the metric, default value or specify.
 *
 * @param T represent metrcic itself, double or int64 ONLY! default is int64.
 * @param label_values Value of Labels, for example "method = get", "get" is label value. You may specify or not.
 *        Amount of label values MUST be the same as label names specifued in Metric.      
 */
template<class T = int64_t>
class Counter: public IBaseMetric
{
    public:
        explicit Counter(std::vector<std::string>&& label_values = {});
        
        void Inc(T delta = 1.0);

        T GetValue() const;
        std::pair<std::string, std::string> GetValueAsString() override;

        /**
         * @brief Dror a counter to zero.
         *
         * Used on for staff in side histogram, you can use in for your aim, but it prohibited
         * in prometheus doc.
         *
         * @param no param
         * @return void
         */
        void Reset();

    private:
        std::atomic<T> m_value{};
};

template<class T>
Counter<T>::Counter(std::vector<std::string>&& label_values) : IBaseMetric(std::move(label_values)) {}

template<class T>
void Counter<T>::Reset()
{
    if constexpr (std::is_same_v<T, double>)
    {
        m_value.store(0.0, std::memory_order_release);
    } 
    else if constexpr (std::is_same_v<T, int64_t>) 
    {
        m_value.store(0, std::memory_order_release);
    }
}

template<class T>
void Counter<T>::Inc(T delta)
{
    T current = 0;
    do
    {
        current = m_value.load(std::memory_order_acquire);
    } 
    while(!m_value.compare_exchange_weak(current, current + delta));
}

template<class T>
T Counter<T>::GetValue() const
{
    return m_value.load(std::memory_order_acquire);
}

template<class T>
std::pair<std::string, std::string> Counter<T>::GetValueAsString()
{
    return std::make_pair(std::to_string(m_value.load(std::memory_order_acquire)), "STUB");
    
}
