#pragma once

#include <type_traits>

#include "init.hpp"

template<class T>
class Counter
{
    public:
        Counter(std::vector<std::string> label_values = {}): m_label_values(std::move(label_values)), m_value(std::is_floating_point<T>::value ? 0.0 : 0){}

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

        std::vector<std::string> Getlabels() 
        {
            return m_label_values;
        }

    private:
        std::vector<std::string> m_label_values;
        std::atomic<T> m_value;
};