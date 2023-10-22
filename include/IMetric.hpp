#pragma once

#include "init.hpp"

struct Info
{
    
};

class IMetric
{
    public:
        IMetric() = default;
        virtual ~IMetric() = default;

    virtual std::vector<std::string> GetLabels() const = 0;
    
    // TESTING STAFF 
    virtual void Show() const = 0;


};