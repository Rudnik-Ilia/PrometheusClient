#pragma once

#include <cstdint>
#include <string>

struct Delete
{
    Delete() = delete;
    Delete(const Delete&) = delete;
    Delete& operator=(const Delete&) = delete;
    Delete(Delete&&) = delete;
    Delete& operator=(Delete&&) = delete;
    ~Delete() = delete;
};

struct NonCopy
{
    NonCopy() = default;
    NonCopy(const NonCopy&) = delete;
    NonCopy(NonCopy&&) = delete;
    NonCopy& operator=(const NonCopy&) = delete;
    NonCopy& operator=(NonCopy&&) = delete;
    ~NonCopy() = default;
};

enum class MetricType : int8_t
{
    COUNTER = 0,
    GAUGE,
    HISTOGRAM,
    SUMMARY,
    UNTYPED
};

inline const std::string COUNTER_STR = "counter";
inline const std::string GAUGE_STR = "gauge";
inline const std::string HISTOGRAM_STR = "histogram";
inline const std::string SUMMARY_STR = "summary";
inline const std::string UNTYPED_STR = "untyped";

inline const std::string HELP = "# HELP ";
inline const std::string TYPE = "# TYPE ";
inline const std::string NEWLINE = "\n";
inline const std::string COMMA = ",";
inline const std::string SPACE = " ";
inline const std::string _TOTAL = "_total";
inline const std::string _BUCKET = "_bucket";
inline const std::string LE = "le=";
inline const std::string _SUM = "_sum";
inline const std::string _COUNT = "_count";
inline const std::string INF = "+Inf";
