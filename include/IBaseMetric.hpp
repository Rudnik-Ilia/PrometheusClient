#pragma once

/**
 * @brief Base class for all metrics.
 *        Enables polymorphic storage in Holder.
 */
class IBaseMetric
{
public:
    explicit IBaseMetric(std::vector<std::string>&& labels_values = {});
    virtual ~IBaseMetric() = default;

    std::vector<std::string> GetLabels() const;
    virtual int64_t GetSize() const;
    virtual std::pair<std::string, std::string> GetValueAsString() = 0;

private:
    std::vector<std::string> m_label_values{};
};

inline IBaseMetric::IBaseMetric(std::vector<std::string>&& labels_values)
    : m_label_values(std::move(labels_values))
{}

inline std::vector<std::string> IBaseMetric::GetLabels() const
{
    return m_label_values;
}

inline int64_t IBaseMetric::GetSize() const
{
    return 1;
}
