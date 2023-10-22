#pragma once

#include <vector>
#include <atomic>
#include <iostream>
#include <memory>

struct Delete
{
    Delete() = delete;
    Delete(const Delete& other) = delete;
    Delete& operator=(const Delete& other) = delete;
    Delete(Delete&& other) = delete;
    Delete&& operator=(Delete&& other) = delete;
    ~Delete() = delete;
};

struct NonCopy
{
    NonCopy() = default;
    NonCopy(const NonCopy& other) = delete;
    NonCopy(NonCopy&& other) = delete;
    NonCopy& operator=(const NonCopy& other) = delete; 
    NonCopy& operator=(NonCopy&& other) = delete; 
    ~NonCopy() = default;
};

enum class MetricType: int8_t
{
    COUNTER = 0,
    GAUGE,
    HISTOGRAM,
    SUMMARY,
    UNTYPED
};


const std::string COUNTER_STR = "counter";
const std::string GAUGE_STR = "gauge";
const std::string HISTOGRAM_STR = "histogram";
const std::string SUMMARY_STR = "summary";
const std::string UNTYPED_STR = "summary";