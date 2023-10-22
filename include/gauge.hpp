#pragma once

#include "init.hpp"
#include "../include/anxilary_functions.hpp"
#include "../include/gauge.hpp"
#include "IBaseMetric.hpp"

typedef std::chrono::hours HOUR;
typedef std::chrono::minutes MINUTE;
typedef std::chrono::seconds SECOND;
typedef std::chrono::milliseconds MILLISEC;
typedef std::chrono::microseconds  MICROSEC;
typedef std::chrono::nanoseconds NANOSEC;

template<class T>
class Gauge: public IBaseMetric
{
    public:
        explicit Gauge(std::vector<std::string>&& labels_values = {}): IBaseMetric(std::move(labels_values)){}

        template <class Measure = SECOND>
        class Timer
        {
            public:
                Timer(Gauge<T>& gauge): m_gauge(gauge), m_start(TimeNow()){}
                ~Timer()
                {
                    m_gauge.Set(TimeNow<Measure>() - m_start);
                }

            private:
                Gauge<T>& m_gauge;
                T m_start;
        };

        template <class Measure = SECOND>
        Timer<Measure> Track()
        {
            return Timer<Measure>(*this);
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

        std::string GetValueAsString()
        {
            return std::to_string(m_value.load(std::memory_order_acquire));
        }

    private:  
        std::atomic<T> m_value{}; 

        template <class Measure = SECOND>
        static T TimeNow()
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            Measure secondsSinceEpoch = std::chrono::duration_cast<Measure>(currentTime.time_since_epoch());
            T unixTime = secondsSinceEpoch.count();
            return unixTime;
        }
};

