#pragma once

#include <type_traits>

#include "init.hpp"
#include "anxilary_functions.hpp"

template<class T = int64_t>
class Counter
{
    public:
        Counter(std::vector<std::string> label_values = {}): m_label_values(std::move(label_values)){}

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
            if(std::is_same<T, double>::value)
            {
                m_value.store(0.0, std::memory_order_release);
            } 
            else if(std::is_same<T, int64_t>::value) 
            {
                m_value.store(0, std::memory_order_release);
            }
        }

        std::vector<std::string> Getlabels() 
        {
            return m_label_values;
        }

    private:
        std::vector<std::string> m_label_values{};
        std::atomic<T> m_value{};
};