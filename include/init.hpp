#pragma once

#include <vector>
#include <atomic>
#include <iostream>
#include <memory>



struct NonCopy
{
    NonCopy() = default;
    NonCopy(const NonCopy& other) = delete;
    NonCopy(NonCopy&& other) = delete;
    NonCopy& operator=(const NonCopy& other) = delete; 
    NonCopy& operator=(NonCopy&& other) = delete; 
};