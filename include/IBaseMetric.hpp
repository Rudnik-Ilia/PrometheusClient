#pragma once

#include "init.hpp"

class IBaseMetric
{
    public:
        explicit IBaseMetric(std::vector<std::string>&& labels_values = {});
        virtual ~IBaseMetric() = default;

        std::vector<std::string> GetLabels() const;
    private:
        std::vector<std::string> m_label_values{};
};

IBaseMetric::IBaseMetric(std::vector<std::string>&& labels_values): m_label_values(std::move(labels_values)){}


std::vector<std::string> IBaseMetric::GetLabels() const
{
    return m_label_values;
}
