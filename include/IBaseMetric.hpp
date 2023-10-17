#pragma once

#include "init.hpp"

class IBaseMetric
{
    public:
        explicit IBaseMetric(std::vector<std::string>&& labels_values = {}): m_label_values(std::move(labels_values)){}
        virtual ~IBaseMetric(){};

        virtual std::vector<std::string> GetLabels()
        {
            return m_label_values;
        } 

    private:
        std::vector<std::string> m_label_values{};
};

