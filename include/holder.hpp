#pragma once

#include "init.hpp"
#include "metric.hpp"
#include "IMetric.hpp"


/**
 * @brief Clas represets a storage for all existing metrics.
 *
 * 
 */

class Holder: public NonCopy
{
    public:
        void RegistrateMetric(std::shared_ptr<IMetric> metric)
        {
            m_storage.push_back(metric);
        }

        size_t GetSize() const
        {
            return m_storage.size();
        }

        void Bridge()
        {
            for(auto& iter : m_storage)
            {
                iter->Collect();
            }
        }

    private:
        std::vector<std::shared_ptr<IMetric>> m_storage{};
};

