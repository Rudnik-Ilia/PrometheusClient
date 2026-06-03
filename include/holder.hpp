#pragma once

#include <sstream>
#include <vector>
#include "init.hpp"
#include "IBaseGroup.hpp"

/**
 * @brief Global registry for all metric groups.
 *        Used as a Singleton.
 */
class Holder : public NonCopy
{
public:
    void RegistrateMetric(std::shared_ptr<IBaseGroup> metric);
    std::string CollectData();
    size_t GetSize() const;

private:
    std::vector<std::shared_ptr<IBaseGroup>> m_storage_group{};
    std::ostringstream m_oss{};
};

