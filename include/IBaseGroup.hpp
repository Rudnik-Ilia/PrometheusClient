#pragma once

#include <string>
#include <vector>

/**
 * @brief Abstract interface for metric groups.
 *        Enables polymorphic serialization in Holder.
 */
class IBaseGroup
{
public:
    IBaseGroup() = default;
    virtual ~IBaseGroup() = default;

    virtual std::vector<std::string> GetLabels() const = 0;
    virtual std::string Serialize() const = 0;
};