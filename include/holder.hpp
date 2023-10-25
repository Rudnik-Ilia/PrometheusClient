#pragma once

#include <sstream>
#include <vector>
#include "init.hpp"
#include "IMetric.hpp"

/**
 * @brief Clas represets a storage for all existing metrics.
 *        This class is Singleton.
 */

class Holder: public NonCopy
{
    public:
        void RegistrateMetric(std::shared_ptr<IMetric> metric);

        std::string GetData();

// FOR TESTING
        size_t GetSize() const;

    private:
        std::vector<std::shared_ptr<IMetric>> m_storage_group{};
        std::ostringstream m_oss{};
};

