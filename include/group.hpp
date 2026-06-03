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
 * @brief Groups metrics of the same type under a common name and help text.
 * @tparam T Metric type (Counter, Gauge, Histogram)
 */
template<class T>
class Group : public IBaseGroup
{
public:
    Group(std::string&& name, std::string&& help, std::vector<std::string>&& label_names = {})
        : m_name(std::move(name))
        , m_help(std::move(help))
        , m_label_names(std::move(label_names))
        , m_type(DefineSelfType())
    {}

    ~Group() = default;

    template <typename... Args>
    std::shared_ptr<T> Build(Args&&... args)
    {
        auto metric = std::make_shared<T>(std::vector<std::string>{std::forward<Args>(args)...});
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
            case MetricType::COUNTER:   return COUNTER_STR;
            case MetricType::GAUGE:     return GAUGE_STR;
            case MetricType::HISTOGRAM: return HISTOGRAM_STR;
            case MetricType::SUMMARY:   return SUMMARY_STR;
            default:                    return UNTYPED_STR;
        }
    }

    std::vector<std::string> GetLabels() const override
    {
        return m_label_names;
    }

    std::string Serialize() const override
    {
        const size_t label_size_group = m_label_names.size();

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

        for (auto& metric : m_storage_metric)
        {
            size_t value_size_metric = metric->GetLabels().size();

            if (label_size_group != value_size_metric)
            {
                continue;
            }

            if (m_type == MetricType::COUNTER || m_type == MetricType::GAUGE)
            {
                SerializeSimpleMetric(result, metric, value_size_metric);
            }
            else if (m_type == MetricType::HISTOGRAM)
            {
                SerializeHistogram(result, metric, value_size_metric);
            }
        }
        return result;
    }

private:
    void SerializeSimpleMetric(
        std::string& result,
        const std::shared_ptr<IBaseMetric>& metric,
        size_t value_size) const
    {
        result += m_name;
        result += "{";

        for (size_t i = 0; i < value_size; ++i)
        {
            result += m_label_names[i];
            result += "=";
            result += AddQuotes(metric->GetLabels()[i]);
            if (i < value_size - 1)
            {
                result += COMMA;
            }
        }

        result += "} ";
        result += metric->GetValueAsString().first;
        result += NEWLINE;
    }

    void SerializeHistogram(
        std::string& result,
        const std::shared_ptr<IBaseMetric>& metric,
        size_t value_size) const
    {
        int64_t bucket_count = metric->GetSize();
        std::pair<std::string, std::string> bucket_value{};

        for (int64_t k = 0; k < bucket_count; ++k)
        {
            result += m_name;
            result += _BUCKET;
            result += "{";

            for (size_t i = 0; i < value_size; ++i)
            {
                result += m_label_names[i];
                result += "=";
                result += AddQuotes(metric->GetLabels()[i]);

                if (i < value_size - 1)
                {
                    result += COMMA;
                }
            }

            bucket_value = metric->GetValueAsString();

            result += COMMA;
            result += LE;
            result += AddQuotes((k == bucket_count - 1) ? INF : bucket_value.first);
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

    MetricType DefineSelfType()
    {
        if constexpr (std::is_same_v<T, Counter<int64_t>> || std::is_same_v<T, Counter<double>>)
        {
            return MetricType::COUNTER;
        }
        else if constexpr (std::is_same_v<T, Gauge<int64_t>> || std::is_same_v<T, Gauge<double>>)
        {
            return MetricType::GAUGE;
        }
        else if constexpr (std::is_same_v<T, Histogram<int64_t>> || std::is_same_v<T, Histogram<double>>)
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
