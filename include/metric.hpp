#pragma once

#include <memory>
#include <cassert>

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
        Metric(std::string&& name, std::string&& help, std::vector<std::string>&& label_names = {}): 
            m_name(std::move(name)), 
            m_help(std::move(help)), 
            m_label_names(std::move(label_names)),
            m_type(DefineSelfType())
            {}

        ~Metric() = default;

        template <typename... Args>
        std::shared_ptr<T> Build(Args&& ...args)
        {
            auto metric = std::make_shared<T>(std::move(std::vector{args})...);
            m_storage.push_back(metric);
            return metric;
        }

        std::shared_ptr<T> AddValues(std::vector<std::string>&& label_values)
        {
            assert(label_values.size() == m_label_names.size());

            auto metric = std::make_shared<T>(std::move(label_values));
            m_storage.push_back(metric);
            return metric;
        }

        MetricType GetType() const
        {
            return m_type;
        }

        std::string GetTypeAsString() const
        {
            switch (m_type)
            {
                case (MetricType::COUNTER):
                    return COUNTER_STR;

                case (MetricType::GAUGE):
                    return GAUGE_STR;

                case (MetricType::HISTOGRAM):
                    return HISTOGRAM_STR;

                default:
                    break;
            }
            return UNTYPED_STR;
        }

        std::vector<std::string> GetLabels() const override
        {
            return m_label_names;
        }


// TESTING STAFF 
        void Show() const override
        {
            std::cout << "# HELP "<< m_help << std::endl;
            std::cout << "# TYPE " << m_name << std::endl;

            for(auto& iter : m_storage)
            {
                std::cout << m_name << " { " << m_label_names[0] << '=' << iter->GetLabels()[0] << " } " << iter->GetValue() << std::endl;
            }
        }
    private:

        MetricType DefineSelfType()
        {
            if constexpr (std::is_same_v<T, Counter<int64_t>> || std::is_same_v<T, Counter<double>>)
            {
                LOG("COUNTER");
                return MetricType::COUNTER;
            }
            else if(std::is_same_v<T, Gauge<int64_t>> || std::is_same_v<T, Gauge<double>>)
            {
                LOG("GAUGE");
                return MetricType::GAUGE;
            }
            else if(std::is_same_v<T, Histogram<int64_t>> || std::is_same_v<T, Histogram<double>>)
            {
                LOG("HISTOGRAM");
                return MetricType::HISTOGRAM;
            }
            LOG("UNTYPED");
            return MetricType::UNTYPED;
        }

        std::string m_name;
        std::string m_help;
        std::vector<std::string> m_label_names{};
        std::vector<std::shared_ptr<T>> m_storage{};

        MetricType m_type;
};

// OLD Builder method

        // Metric<T>& Name(std::string&& name)
        // {
        //     m_name = (std::move(name));
        //     return *this;
        // }

        // Metric<T>& Help(std::string&& help)
        // {
        //     m_help = (std::move(help));
        //     return *this;
        // }

        // Metric<T>& Labels(std::vector<std::string>&& label_names)
        // {
        //     m_label_names = (std::move(label_names));
        //     return *this;
        // }
// ********************888
        // template <typename... Args>
        // std::shared_ptr<T> AddValues(Args&& ...args)
        // {
        //     auto metric = std::make_shared<T>(std::move(std::vector{args})...);
        //     m_storage.push_back(metric);
        //     return metric;
        // }