#pragma once

#include "init.hpp"

class IMetric
{
    public:
    IMetric() = default;
    virtual ~IMetric() = default;

    virtual std::vector<std::string> GetLabels() const = 0;
};