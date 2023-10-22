#pragma once

#include "init.hpp"

struct InfoGroup
{   
    std::string info_help{};
    std::string info_name{};
    std::string info_type{};
    std::vector<std::string> info_labels{};
};

struct InfoMetric
{   
    std::vector<std::string> info_names{};
    std::string info_value{};
};

class IMetric: public InfoGroup
{
    public:
        IMetric() = default;
        virtual ~IMetric() = default;

    virtual std::vector<std::string> GetLabels() const = 0;

    virtual void Collect() = 0;

    // TESTING STAFF 
    virtual void Show() const = 0;

    private:

};