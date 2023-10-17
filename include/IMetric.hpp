#pragma once

#include "init.hpp"

class IMetric
{
    public:
    IMetric(){}
    virtual ~IMetric(){};

    virtual std::vector<std::string> GetLabels() = 0;
};