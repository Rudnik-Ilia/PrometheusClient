#pragma once

#include "init.hpp"
#include "../include/anxilary_functions.hpp"

class Gauge
{
    public:
        Gauge(std::vector<std::string> labels_values = {}): m_label_values(std::move(labels_values)){}
        
        class Timer
        {
            public:
                Timer(Gauge& gauge): m_gauge(gauge), m_start(TimeNow()){}
                ~Timer()
                {
                    m_gauge.Set(TimeNow() - m_start);
                }

            private:
                Gauge& m_gauge;
                int64_t m_start;
        };

        Timer Duration()
        {
            return Timer(*this);
        }

        void Inc(int64_t delta = 1)
        {
            m_value.fetch_add(delta);
        }

        void Dec(int64_t delta = 1)
        {
            m_value.fetch_sub(delta);
        }

        void Set(int64_t value)
        {
            m_value.store(value);
        }

        void SetTimeNow()
        {
            Set(TimeNow());
        }

        int64_t GetValue()
        {
            return m_value.load();
        }

        std::vector<std::string> GetLabels()
        {
            return m_label_values;
        }

    private:  
        std::vector<std::string> m_label_values;
        std::atomic<int64_t> m_value{0};  
};