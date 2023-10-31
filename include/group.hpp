#pragma once

#include <memory>
#include <cassert>
#include <sstream>

#include "init.hpp"
#include "IBaseGroup.hpp"
#include "counter.hpp"
#include "gauge.hpp"
#include "histogram.hpp"

/**
 * @brief Group of metrics.
 *        Accumulate several metric with the same type under the same name and help. 
 *
 * @param T specify the desireble type of metric....Counter , Gauge, Histogram, Summary.
 * @param name of metric
 * @param Some info about metric, what for. 
 * @param Name of labels for metric. for example "method = post"..."method" is a label name. 
 */

template<class T>
class Group : public IBaseGroup
{
    public:
        Group(std::string&& name, std::string&& help, std::vector<std::string>&& label_names = {}): 
            m_name(std::move(name)), 
            m_help(std::move(help)), 
            m_label_names(std::move(label_names)),
            m_type(DefineSelfType())
            {}

        ~Group() = default;

        template <typename... Args>
        std::shared_ptr<T> Build(Args&& ...args)
        {
            auto metric = std::make_shared<T>(std::move(std::vector{args})...);
            m_storage_metric.push_back(metric);
            return metric;
        }

        std::shared_ptr<T> AddValues(std::vector<std::string>&& label_values)
        {
            assert(label_values.size() == m_label_names.size());

            auto metric = std::make_shared<T>(std::move(label_values));
            m_storage_metric.push_back(metric);
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

        std::string Serialize() const override
        {
            const size_t label_size_group = m_label_names.size();
            size_t value_size_metric = 0;

            std::string result;
            result.reserve(1024);

            result += HELP;
            result += m_name;
            result += SPACE;
            result += m_help;
            result += NEWLINE;
            result += TYPE;
            result += m_name;
            result += SPACE;
            result += GetTypeAsString();
            result += NEWLINE;

            for(auto& metric : m_storage_metric)
            {
                if((this->m_type == MetricType::COUNTER) || (this->m_type == MetricType::GAUGE))
                {
                    value_size_metric = metric->GetLabels().size();

                    if(label_size_group != value_size_metric)
                    {
                        continue;
                    }

                    result += m_name;
                    result += "{";

                    for(size_t i = 0; i < value_size_metric; ++i)
                    {
                        result += m_label_names[i];
                        result += "=";
                        result += AddQuotes(metric->GetLabels()[i]);
                        if(i < (value_size_metric - 1))
                        {
                            result += COMMA;
                        }
                    }

                    result += "} ";
                    result += metric->GetValueAsString().first;
                    result += NEWLINE;
                }
                else if(this->m_type == MetricType::HISTOGRAM)
                {
                    value_size_metric = metric->GetLabels().size();

                    if(label_size_group != value_size_metric)
                    {
                        continue;
                    }

                    int64_t size_of_buckets = metric->GetSize();

                    std::pair<std::string, std::string> bucket_value{};

                    for(size_t k = 0; k < size_of_buckets; ++k)
                    {
                        result += m_name;
                        result += _BUCKET;
                        result += "{";

                        for(size_t i = 0; i < value_size_metric; ++i)
                        {
                            result += m_label_names[i];
                            result += "=";
                            result += AddQuotes(metric->GetLabels()[i]);

                            if(i < (value_size_metric - 1))
                            {
                                result += COMMA;
                            }
                        }

                        bucket_value = metric->GetValueAsString(); 
                        
                        result += COMMA;
                        result += LE;

                        result += AddQuotes((k == size_of_buckets - 1) ? INF : bucket_value.first);

                        result += "} ";
                        result += bucket_value.second;
                        result += NEWLINE;
                    }

                    result += m_name;
                    result += _SUM;
                    result += SPACE;
                    result += bucket_value.first;
                    result += NEWLINE;
                    result += m_name;
                    result += _COUNT;
                    result += SPACE;
                    result += bucket_value.second;
                    result += NEWLINE;
                }
            }
            return result;
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
        std::vector<std::shared_ptr<IBaseMetric>> m_storage_metric{};

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
        //     m_storage_metric.push_back(metric);
        //     return metric;
        // }

// *****************************************************
    // for(size_t j = 0; j < m_label_names.size(); ++j)
    // {
    //     std::cout << m_name << " {";  

    //     for(size_t i = 0; i < m_storage_metric.size(); ++i)
    //     {
    //         std::cout << "LOOP: " << i << std::endl;
    //         std::cout << m_label_names[i] << " = " << '"'<< m_storage_metric[j]->GetLabels()[i] << '"' << ",";
    //     }
    //     std::cout << " }"; 
    //     std::cout << m_storage_metric[j]->GetValueAsString();
    //     std::cout << '\n';
    // }