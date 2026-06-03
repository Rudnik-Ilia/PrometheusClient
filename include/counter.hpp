#pragma once

#include <type_traits>

#include "anxilary_functions.hpp"
#include "IBaseMetric.hpp"

/**
 * @brief Counter metric - can only be incremented.
 * @tparam T Value type: double or int64_t (default: int64_t)
 */
template<class T = int64_t>
class Counter : public IBaseMetric
{
public:
    explicit Counter(std::vector<std::string>&& label_values = {});

    void Inc(T delta = 1.0);
    void Reset();

    T GetValue() const;
    std::pair<std::string, std::string> GetValueAsString() override;

private:
    std::atomic<T> m_value{};
};

template<class T>
Counter<T>::Counter(std::vector<std::string>&& label_values)
    : IBaseMetric(std::move(label_values))
{}

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
    while (!m_value.compare_exchange_weak(current, current + delta));
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
