#include <gtest/gtest.h>
#include <memory>

#include "histogram.hpp"

TEST(HistogramTest, DefaultBucketIsInfinity)
{
    Histogram<double> h;
    EXPECT_EQ(h.GetSize(), 1);
}

TEST(HistogramTest, ManualBuckets)
{
    Histogram<double> h;
    h.Buckets({1.0, 2.0, 3.0});
    EXPECT_EQ(h.GetSize(), 4);
}

TEST(HistogramTest, LinearBuckets)
{
    Histogram<double> h;
    h.LinearBuckets(1.0, 1.0, 3);
    EXPECT_EQ(h.GetSize(), 4);
}

TEST(HistogramTest, ExponentialBuckets)
{
    Histogram<double> h;
    h.ExponentialBuckets(1.0, 2.0, 3);
    EXPECT_EQ(h.GetSize(), 4);
}

TEST(HistogramTest, ObserveUpdatesSum)
{
    Histogram<double> h;
    h.Buckets({1.0, 2.0, 3.0});

    h.Observe(0.5);
    h.Observe(1.5);
    h.Observe(2.5);

    EXPECT_DOUBLE_EQ(h.GetValue(), 4.5);
}

TEST(HistogramTest, Labels)
{
    Histogram<double> h(std::vector<std::string>{"latency", "handler"});
    auto labels = h.GetLabels();
    ASSERT_EQ(labels.size(), 2);
    EXPECT_EQ(labels[0], "latency");
    EXPECT_EQ(labels[1], "handler");
}

TEST(HistogramTest, Reset)
{
    Histogram<double> h;
    h.Buckets({1.0, 2.0});

    h.Observe(0.5);
    h.Observe(1.5);
    h.Reset();

    EXPECT_EQ(h.GetSize(), 3);
}

TEST(HistogramTest, Int64Histogram)
{
    Histogram<int64_t> h;
    h.Buckets({10, 20, 30});

    h.Observe(5);
    h.Observe(15);
    h.Observe(25);
    h.Observe(35);

    EXPECT_EQ(h.GetValue(), 80);
}
