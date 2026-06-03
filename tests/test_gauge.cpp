#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#include "gauge.hpp"

TEST(GaugeTest, DefaultValueIsZero)
{
    Gauge<int64_t> gauge;
    EXPECT_EQ(gauge.GetValue(), 0);
}

TEST(GaugeTest, Increment)
{
    Gauge<int64_t> gauge;
    gauge.Inc(10);
    EXPECT_EQ(gauge.GetValue(), 10);
}

TEST(GaugeTest, Decrement)
{
    Gauge<int64_t> gauge;
    gauge.Set(100);
    gauge.Dec(30);
    EXPECT_EQ(gauge.GetValue(), 70);
}

TEST(GaugeTest, Set)
{
    Gauge<int64_t> gauge;
    gauge.Set(42);
    EXPECT_EQ(gauge.GetValue(), 42);
}

TEST(GaugeTest, IncAndDec)
{
    Gauge<int64_t> gauge;
    gauge.Inc(50);
    gauge.Dec(20);
    gauge.Inc(10);
    EXPECT_EQ(gauge.GetValue(), 40);
}

TEST(GaugeTest, DoubleGauge)
{
    Gauge<double> gauge;
    gauge.Set(3.14);
    gauge.Inc(0.86);
    EXPECT_DOUBLE_EQ(gauge.GetValue(), 4.0);
}

TEST(GaugeTest, NegativeValues)
{
    Gauge<int64_t> gauge;
    gauge.Set(-10);
    EXPECT_EQ(gauge.GetValue(), -10);

    gauge.Dec(5);
    EXPECT_EQ(gauge.GetValue(), -15);
}

TEST(GaugeTest, Labels)
{
    Gauge<int64_t> gauge(std::vector<std::string>{"cpu", "idle"});
    auto labels = gauge.GetLabels();
    ASSERT_EQ(labels.size(), 2);
    EXPECT_EQ(labels[0], "cpu");
    EXPECT_EQ(labels[1], "idle");
}

TEST(GaugeTest, Timer)
{
    Gauge<int64_t> gauge;

    {
        auto timer = gauge.Track<MILLISEC>();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    EXPECT_GE(gauge.GetValue(), 90);
    EXPECT_LE(gauge.GetValue(), 200);
}

TEST(GaugeTest, GetValueAsString)
{
    Gauge<int64_t> gauge;
    gauge.Set(123);
    auto result = gauge.GetValueAsString();
    EXPECT_EQ(result.first, "123");
}
