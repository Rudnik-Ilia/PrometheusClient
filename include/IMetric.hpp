#pragma once

#include "init.hpp"



class IMetric
{
    public:
        IMetric() = default;
        virtual ~IMetric() = default;

    virtual std::vector<std::string> GetLabels() const = 0;

    virtual void Collect() = 0;

    // TESTING STAFF 
    virtual std::string Serialize() const = 0;

    private:

};