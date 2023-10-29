#pragma once

/**
 * @brief Base class for all metrics.
 *
 * use in a Holder class to store dffrent type of metrics in the same container.
 */

class IBaseMetric
{
    public:
        explicit IBaseMetric(std::vector<std::string>&& labels_values = {});
        virtual ~IBaseMetric() = default;

        std::vector<std::string> GetLabels() const;

        virtual std::pair<std::string, std::string> GetValueAsString() = 0;

    private:
        std::vector<std::string> m_label_values{};
};

IBaseMetric::IBaseMetric(std::vector<std::string>&& labels_values): m_label_values(std::move(labels_values)){}

std::vector<std::string> IBaseMetric::GetLabels() const
{
    return m_label_values;
}
