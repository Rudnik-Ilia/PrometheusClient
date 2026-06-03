#include "../include/holder.hpp"

void Holder::RegistrateMetric(std::shared_ptr<IBaseGroup> metric)
{
    m_storage_group.push_back(std::move(metric));
}

size_t Holder::GetSize() const
{
    return m_storage_group.size();
}

std::string Holder::CollectData()
{
    for (const auto& group : m_storage_group)
    {
        m_oss << group->Serialize();
    }

    std::string result = m_oss.str();

    m_oss.str("");
    m_oss.clear();

    return result;
}
