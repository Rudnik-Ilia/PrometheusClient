#pragma once

#include "init.hpp"
#include "metric.hpp"
#include "IMetric.hpp"

class Holder: public NonCopy
{
    public:

        void RegistrateMetric(std::shared_ptr<IMetric> metric)
        {
            m_storage.push_back(metric);
        }

        size_t GetSize()
        {
            return m_storage.size();
        }

    private:
        std::vector<std::shared_ptr<IMetric>> m_storage{};
};

