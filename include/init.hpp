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
};

enum class MetricType
{
    COUNTER = 0,
    GAUGE,
    HISTOGRAM,
    SUMMARY,
    UNTYPED
};