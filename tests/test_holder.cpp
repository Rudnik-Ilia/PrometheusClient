#include <gtest/gtest.h>
#include <string>

#include "prometheus.hpp"

class HolderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        holder = Singleton<Holder>::GetInstance();
    }

    Holder* holder;
};

TEST_F(HolderTest, RegisterCounter)
{
    size_t initial_size = holder->GetSize();

    auto counter = prometheus<Counter, int64_t>::GetMetric(
        "test_counter_" + std::to_string(initial_size),
        "Test counter metric",
        {"method"},
        {"GET"});

    EXPECT_EQ(holder->GetSize(), initial_size + 1);
}

TEST_F(HolderTest, RegisterGauge)
{
    size_t initial_size = holder->GetSize();

    auto gauge = prometheus<Gauge, double>::GetMetric(
        "test_gauge_" + std::to_string(initial_size),
        "Test gauge metric",
        {"cpu"},
        {"idle"});

    EXPECT_EQ(holder->GetSize(), initial_size + 1);
}

TEST_F(HolderTest, CollectDataContainsHelp)
{
    auto counter = prometheus<Counter, int64_t>::GetMetric(
        "collect_test_counter",
        "Help text for testing",
        {},
        {});

    counter->Inc(42);

    std::string output = holder->CollectData();
    EXPECT_TRUE(output.find("# HELP collect_test_counter") != std::string::npos);
}

TEST_F(HolderTest, CollectDataContainsType)
{
    auto counter = prometheus<Counter, int64_t>::GetMetric(
        "type_test_counter",
        "Type test",
        {},
        {});

    std::string output = holder->CollectData();
    EXPECT_TRUE(output.find("# TYPE type_test_counter counter") != std::string::npos);
}

TEST_F(HolderTest, GroupWithMultipleMetrics)
{
    size_t initial_size = holder->GetSize();

    auto group = prometheus<Counter, int64_t>::GetGroup(
        "http_requests_" + std::to_string(initial_size),
        "Total HTTP requests",
        {"method", "status"});

    auto get_200 = group->AddValues({"GET", "200"});
    auto post_201 = group->AddValues({"POST", "201"});
    auto get_404 = group->AddValues({"GET", "404"});

    get_200->Inc(100);
    post_201->Inc(50);
    get_404->Inc(5);

    EXPECT_EQ(get_200->GetValue(), 100);
    EXPECT_EQ(post_201->GetValue(), 50);
    EXPECT_EQ(get_404->GetValue(), 5);
}

TEST_F(HolderTest, HistogramSerialization)
{
    auto group = prometheus<Histogram, double>::GetGroup(
        "request_duration",
        "Request duration in seconds",
        {"handler"});

    auto h = group->AddValues({"main"});
    h->Buckets({0.1, 0.5, 1.0});

    h->Observe(0.05);
    h->Observe(0.3);
    h->Observe(0.8);

    std::string output = holder->CollectData();
    EXPECT_TRUE(output.find("request_duration_bucket") != std::string::npos);
    EXPECT_TRUE(output.find("request_duration_sum") != std::string::npos);
    EXPECT_TRUE(output.find("request_duration_count") != std::string::npos);
}
