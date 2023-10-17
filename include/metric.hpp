#pragma once

#include <memory>

#include "init.hpp"
#include "counter.hpp"
#include "holder.hpp"
#include "IMetric.hpp"
#include "gauge.hpp"
#include "histogram.hpp"

template<class T>
class Metric : public IMetric
{
    public:
        explicit Metric(){}

        Metric(std::string&& name, std::string&& help, std::vector<std::string>&& label_names = {}): 
        m_name(std::move(name)), 
        m_help(std::move(help)), 
        m_label_names(std::move(label_names)),
        m_type(DefineSelfType())
        {}

        ~Metric() = default;

        Metric<T>& Name(std::string&& name)
        {
            m_name = (std::move(name));
            return *this;
        }

        Metric<T>& Help(std::string&& help)
        {
            m_help = (std::move(help));
            return *this;
        }

        Metric<T>& Labels(std::vector<std::string>&& label_names)
        {
            m_label_names = (std::move(label_names));
            return *this;
        }

        void Register(Holder& holder)
        {
            
        }

        template <typename... Args>
        std::shared_ptr<T> Build(Args&& ...args)
        {
            auto metric = std::make_shared<T>(std::move(std::vector{args})...);
            m_storage.push_back(metric);
            return metric;
        }

        template <typename... Args>
        std::shared_ptr<T> AddValues(Args&& ...args)
        {
            auto metric = std::make_shared<T>(std::move(std::vector{args})...);
            m_storage.push_back(metric);
            return metric;
        }

        MetricType GetType() const
        {
            return m_type;
        }

// TESTING STAFF 
        std::vector<std::string> GetLabels() const override
        {
            return m_label_names;
        }
        void Show()
        {
            for(auto& iter : m_storage)
            {
                std::cout << m_name << ' ' << m_label_names[0] << '=' << iter->GetLabels()[0] << " { " << iter->GetValue() << " }"<< std::endl;
            }
        }
    private:

        MetricType DefineSelfType()
        {
            if constexpr (std::is_same_v<T, Counter<int64_t>>)
            {
                LOG("Counter------");
                return MetricType::COUNTER;
            }
            else if(std::is_same_v<T, Gauge<int64_t>>)
            {
                LOG("GAUGE------");
                return MetricType::GAUGE;
            }
            else if(std::is_same_v<T, Histogram<T>>)
            {
                LOG("Histo------");
                return MetricType::HISTOGRAM;
            }
            LOG("Untyped------");
            return MetricType::UNTYPED;
        }

        std::string m_name;
        std::string m_help;
        std::vector<std::string> m_label_names;
        std::vector<std::shared_ptr<T>> m_storage;

        MetricType m_type;
};

