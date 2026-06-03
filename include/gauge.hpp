#pragma once

#include "anxilary_functions.hpp"
#include "IBaseMetric.hpp"

using HOUR = std::chrono::hours;
using MINUTE = std::chrono::minutes;
using SECOND = std::chrono::seconds;
using MILLISEC = std::chrono::milliseconds;
using MICROSEC = std::chrono::microseconds;
using NANOSEC = std::chrono::nanoseconds;

/**
 * @brief Gauge metric - can increase, decrease, or be set to arbitrary values.
 *        Supports time tracking via RAII Timer.
 * @tparam T Value type: double or int64_t (default: int64_t)
 */
template<class T = int64_t>
class Gauge : public IBaseMetric
{
public:
    explicit Gauge(std::vector<std::string>&& labels_values = {});

    template <class Measure = SECOND>
    class Timer
    {
    public:
        explicit Timer(Gauge<T>& gauge);
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
    std::pair<std::string, std::string> GetValueAsString() override;

private:
    std::atomic<T> m_value{};

    template <class Measure = SECOND>
    static T TimeNow()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<Measure>(currentTime.time_since_epoch());
        return static_cast<T>(duration.count());
    }
};

template<typename T>
Gauge<T>::Gauge(std::vector<std::string>&& labels_values)
    : IBaseMetric(std::move(labels_values))
{}

template<typename T>
void Gauge<T>::Inc(T delta)
{
    T current = 0;
    do
    {
        current = m_value.load(std::memory_order_acquire);
    }
    while (!m_value.compare_exchange_weak(current, current + delta));
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
std::pair<std::string, std::string> Gauge<T>::GetValueAsString()
{
    return std::make_pair(std::to_string(m_value.load(std::memory_order_acquire)), "STUB");
}

template<typename T>
template <class Measure>
Gauge<T>::Timer<Measure> Gauge<T>::Track()
{
    return Timer<Measure>(*this);
}

template<typename T>
template <class Measure>
Gauge<T>::Timer<Measure>::Timer(Gauge<T>& gauge)
    : m_gauge(gauge)
    , m_start(TimeNow<Measure>())
{}

template<typename T>
template <class Measure>
Gauge<T>::Timer<Measure>::~Timer()
{
    m_gauge.Set(TimeNow<Measure>() - m_start);
}
