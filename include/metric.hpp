#pragma once

#include <memory>
#include <cassert>

#include "init.hpp"
#include "counter.hpp"
#include "holder.hpp"
#include "IMetric.hpp"
#include "gauge.hpp"
#include "histogram.hpp"

/**
 * @brief Group of metrics.
 *        Accumulate several metric with the same type under the same name and help. 
 *
 * @param T specify the desireble type of metric....counter , gauge...etc.
 * @param name of metric
 * @param Some info about metric, what for. 
 * @param Name of labels for metric. for example "method = post"..."method" is a label name. 
 */

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

                case (MetricType::SUMMARY):
                    return SUMMARY_STR;

                default:
                    break;
            }
            return UNTYPED_STR;
        }

        std::vector<std::string> GetLabels() const override
        {
            return m_label_names;
        }

        void Collect() override
        {
            std::cout << "SIZE STORAGE: " << m_storage.size() << std::endl;
            std::cout << "# HELP "<< m_help << std::endl;
            std::cout << "# TYPE " << m_name << ' ' << GetTypeAsString() << std::endl;
            Show();
        }

        /**
         * @brief Only for tests.
         *
         * @param no param
         * @return void and print inside.
         */
        void Show() const override
        {
            for(size_t j = 0; j < m_label_names.size(); ++j)
            {
                std::cout << m_name << " {";  

                for(size_t i = 0; i < m_storage.size(); ++i)
                {
                    std::cout << "LOOP: " << i << std::endl;
                    std::cout << m_label_names[i] << " = " << '"'<< m_storage[j]->GetLabels()[i] << '"' << ",";
                }
                std::cout << " }"; 
                std::cout << m_storage[j]->GetValueAsString();
                std::cout << '\n';
            }
        }
    private:

        MetricType DefineSelfType()
        {
            if constexpr (std::is_same_v<T, Counter<int64_t>> || std::is_same_v<T, Counter<double>>)
            {
                return MetricType::COUNTER;
            }
            else if(std::is_same_v<T, Gauge<int64_t>> || std::is_same_v<T, Gauge<double>>)
            {
                return MetricType::GAUGE;
            }
            else if(std::is_same_v<T, Histogram<int64_t>> || std::is_same_v<T, Histogram<double>>)
            {
                return MetricType::HISTOGRAM;
            }
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