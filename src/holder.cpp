
#include <memory>
#include <vector>

#include "../include/holder.hpp"

void Holder::RegistrateMetric(std::shared_ptr<IBaseGroup> metric)
{
    m_storage_group.push_back(metric);
}

size_t Holder::GetSize() const
{
    return m_storage_group.size();
}

std::string Holder::CollectData()
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
