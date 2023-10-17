#pragma once

#include <type_traits>

#include "init.hpp"
#include "anxilary_functions.hpp"
#include "IBaseMetric.hpp"

template<class T = int64_t>
class Counter: public IBaseMetric
{
    public:
        explicit Counter(std::vector<std::string>&& label_values = {}): IBaseMetric(std::move(label_values)) {}
        
        void Inc(T delta = 1)
        {
            T current = 0;
            do
            {
                current = m_value.load(std::memory_order_acquire);
            } 
            while(!m_value.compare_exchange_weak(current, current + delta));
        }

        T GetValue()
        {
            return m_value.load(std::memory_order_acquire);
        }

        void Reset()
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

    private:
        std::atomic<T> m_value{};
};