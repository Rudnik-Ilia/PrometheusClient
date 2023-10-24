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

/**
 * @brief Class represent a Gauge metric.
 *
 * You can increase and decrease the metric, also you can measure a period of time by Track<>,
 * specify measurment units that written above.
 *
 * @param T represent metrcic type value, double or int64 ONLY! default is int64.
 * @param label_values Value of Labels, for example "method = get", "get" is label value. You may specify or not.
 *        Amount of label values MUST be the same as label names specifued in Metric.      
 */
template<class T = int64_t>
class Gauge: public IBaseMetric
{
    public:
        explicit Gauge(std::vector<std::string>&& labels_values = {});

        template <class Measure = SECOND>
        class Timer
        {
            public:
                Timer(Gauge<T>& gauge);
                ~Timer();
            private:
                Gauge<T>& m_gauge;
                T m_start;
        };

        template <class Measure = SECOND>
        Timer<Measure> Track();

        void Inc(T delta = 1.0);

        void Dec(T delta = 1);

        void Set(T value);

        void SetTimeNow();

        T GetValue() const;

        std::string GetValueAsString() const;

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

template<typename T>
Gauge<T>::Gauge(std::vector<std::string>&& labels_values) : IBaseMetric(std::move(labels_values)){}

template<typename T>
void Gauge<T>::Inc(T delta)
{
    T current = 0;
    do
    {
        current = m_value.load(std::memory_order_acquire);
    } 
    while(!m_value.compare_exchange_weak(current, current + delta)); 
}

template<typename T>
void Gauge<T>::Dec(T delta)
{
    Inc(-delta);
}

template<typename T>
void Gauge<T>::Set(T value)
{
    m_value.store(value, std::memory_order_release);
}

template<typename T>
void Gauge<T>::SetTimeNow()
{
    Set(TimeNow());
}

template<typename T>
T Gauge<T>::GetValue() const
{
    return m_value.load(std::memory_order_release);
}

template<typename T>
std::string Gauge<T>::GetValueAsString() const
{
    return std::to_string(m_value.load(std::memory_order_acquire));
}

template<typename T>
template <class Measure>
Gauge<T>::Timer<Measure> Gauge<T>::Track()
{
    return Timer<Measure>(*this);
}

template<typename T>
template <class Measure>
Gauge<T>::Timer<Measure>::Timer(Gauge<T>& gauge): m_gauge(gauge), m_start(TimeNow()){}

template<typename T>
template <class Measure>
Gauge<T>::Timer<Measure>::~Timer()
{
    m_gauge.Set(TimeNow<Measure>() - m_start);
}

