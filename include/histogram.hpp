#pragma once

#include "init.hpp"


class Histogram
{
    public:

        using BucketsLimit = std::vector<double>;

        Histogram(std::vector<std::string> label_values, BucketsLimit): m_label_values(std::move(label_values)){}

        struct Descriptor 
        {
            std::vector<double> bucketBounds;
        };

        struct Bucket 
        {
            double upperBound;
            std::atomic<double> m_bucket_count{0.0};
        };

        static std::vector<double> defaultBuckets()
        {

        }

        static std::vector<double> linearBuckets(double start, double width, uint64_t count)
        {

        }

        static std::vector<double> exponentialBuckets(double start, double factor, uint64_t count)
        {

        }


        void Observe(double val)
        {

        }

        double _summ()
        {

        }

        double _count()
        {

        }

    private:
        std::vector<std::string> m_label_values;
        std::atomic<double> m_sum{0.0};
        std::vector<Bucket> m_buckets;
};
