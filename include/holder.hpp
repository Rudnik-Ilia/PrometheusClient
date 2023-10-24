#pragma once

#include <sstream>

#include "init.hpp"
#include "metric.hpp"
#include "IMetric.hpp"

/**
 * @brief Clas represets a storage for all existing metrics.
 *        This class is Singleton.
 */

class Holder: public NonCopy
{
    public:
        void RegistrateMetric(std::shared_ptr<IMetric> metric)
        {
            m_storage_group.push_back(metric);
        }

        size_t GetSize() const
        {
            return m_storage_group.size();
        }

        void Bridge()
        {
            for(auto& group : m_storage_group)
            {
                group->Collect();
            }
        }

        std::string GetData()
        {
            std::string result;

            for(auto& group : m_storage_group)
            {
                m_oss << group->Serialize();
            }

            result = m_oss.str();
            
            m_oss.str("");
            m_oss.clear();

            return result;
        }

    private:
        std::vector<std::shared_ptr<IMetric>> m_storage_group{};
        std::ostringstream m_oss{};

};

