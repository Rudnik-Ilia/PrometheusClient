#pragma once

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
const std::string UNTYPED_STR = "untyped";


const std::string HELP = "# HELP ";
const std::string TYPE = "# TYPE ";
const std::string NEWLINE = "\n";
const std::string COMMA = ",";
const std::string SPACE = " ";
const std::string _TOTAL = "_total";
const std::string _BUCKET = "_bucket";
const std::string LE = "le=";
const std::string _SUM = "_sum";
const std::string _COUNT = "_count";
const std::string INF = "+Inf";
