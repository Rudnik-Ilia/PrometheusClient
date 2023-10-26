#pragma once

#include <iostream>
#include <vector>


class IBaseGroup
{
    public:
        IBaseGroup() = default;
        virtual ~IBaseGroup() = default;

    virtual std::vector<std::string> GetLabels() const = 0;
    virtual std::string Serialize() const = 0;
};