#pragma once

#include <sstream>
#include <vector>
#include "init.hpp"
#include "IBaseGroup.hpp"

/**
 * @brief Clas represets a storage for all existing metrics.
 *        This class is Singleton.
 */

class Holder: public NonCopy
{
    public:
        void RegistrateMetric(std::shared_ptr<IBaseGroup> metric);

        std::string CollectData();

// FOR TESTING
        size_t GetSize() const;

    private:
        std::vector<std::shared_ptr<IBaseGroup>> m_storage_group{};
        std::ostringstream m_oss{};
};

