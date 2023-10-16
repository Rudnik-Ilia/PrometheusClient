#pragma once

#include "init.hpp"


class IBaseMetric
{
    public:
        explicit IBaseMetric(std::vector<std::string> labels_values = {}): m_label_values(labels_values) {}
        virtual ~IBaseMetric() = default;

    private:
        std::vector<std::string> m_label_values;
};

