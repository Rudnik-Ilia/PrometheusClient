#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "counter.hpp"

TEST(CounterTest, DefaultValueIsZero)
{
    Counter<int64_t> counter;
    EXPECT_EQ(counter.GetValue(), 0);
}

TEST(CounterTest, IncrementByOne)
{
    Counter<int64_t> counter;
    counter.Inc();
    EXPECT_EQ(counter.GetValue(), 1);
}

TEST(CounterTest, IncrementByValue)
{
    Counter<int64_t> counter;
    counter.Inc(5);
    EXPECT_EQ(counter.GetValue(), 5);
}

TEST(CounterTest, MultipleIncrements)
{
    Counter<int64_t> counter;
    counter.Inc(10);
    counter.Inc(20);
    counter.Inc(30);
    EXPECT_EQ(counter.GetValue(), 60);
}

TEST(CounterTest, DoubleCounter)
{
    Counter<double> counter;
    counter.Inc(1.5);
    counter.Inc(2.5);
    EXPECT_DOUBLE_EQ(counter.GetValue(), 4.0);
}

TEST(CounterTest, Reset)
{
    Counter<int64_t> counter;
    counter.Inc(100);
    counter.Reset();
    EXPECT_EQ(counter.GetValue(), 0);
}

TEST(CounterTest, Labels)
{
    Counter<int64_t> counter(std::vector<std::string>{"GET", "200"});
    auto labels = counter.GetLabels();
    ASSERT_EQ(labels.size(), 2);
    EXPECT_EQ(labels[0], "GET");
    EXPECT_EQ(labels[1], "200");
}

TEST(CounterTest, ThreadSafety)
{
    Counter<int64_t> counter;
    const int num_threads = 4;
    const int increments_per_thread = 100000;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back([&counter, increments_per_thread]() {
            for (int j = 0; j < increments_per_thread; ++j)
            {
                counter.Inc();
            }
        });
    }

    for (auto& t : threads)
    {
        t.join();
    }

    EXPECT_EQ(counter.GetValue(), num_threads * increments_per_thread);
}

TEST(CounterTest, GetValueAsString)
{
    Counter<int64_t> counter;
    counter.Inc(42);
    auto result = counter.GetValueAsString();
    EXPECT_EQ(result.first, "42");
}
