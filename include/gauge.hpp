#pragma once

#include "init.hpp"
#include "../include/anxilary_functions.hpp"
#include "../include/gauge.hpp"

typedef std::chrono::hours HOURS;
typedef std::chrono::minutes MINUTE;
typedef std::chrono::seconds SECOND;
typedef std::chrono::milliseconds MILLISEC;
typedef std::chrono::microseconds  MICROSEC;
typedef std::chrono::nanoseconds NANOSEC;

template<class T, class Measure = SECOND>
class Gauge
{
    public:
        Gauge(std::vector<std::string> labels_values = {}): m_label_values(std::move(labels_values)), m_value(0){}
        
        class Timer
        {
            public:
                Timer(Gauge<T, Measure>& gauge): m_gauge(gauge), m_start(TimeNow()){}
                ~Timer()
                {
                    m_gauge.Set(TimeNow() - m_start);
                }

            private:
                Gauge<T, Measure>& m_gauge;
                T m_start;
        };

        Timer Track()
        {
            return Timer(*this);
        }

        void Inc(T delta = 1.0)
        {
            T current = 0;
            do
            {
                current = m_value.load(std::memory_order_acquire);
            } 
            while(!m_value.compare_exchange_weak(current, current + delta));
            
        }

        void Dec(T delta = 1)
        {
            Inc(-delta);
        }

        void Set(T value)
        {
            m_value.store(value, std::memory_order_release);
        }

        void SetTimeNow()
        {
            Set(TimeNow());
        }

        T GetValue()
        {
            return m_value.load(std::memory_order_release);
        }

        std::vector<std::string> GetLabels()
        {
            return m_label_values;
        }

    private:  
        std::vector<std::string> m_label_values;
        std::atomic<T> m_value; 

        static T TimeNow()
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            Measure secondsSinceEpoch = std::chrono::duration_cast<Measure>(currentTime.time_since_epoch());
            T unixTime = secondsSinceEpoch.count();
            return unixTime;
        }
};

