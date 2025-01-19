#include <cmath>

#include "caslib/analyze.hpp"
#include "caslib/series.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(Analyze, Stats) {
    Series data{3, 0, 4, -2, 3};
    EXPECT_FLOAT_EQ(Mean(data), 1.6);
    EXPECT_FLOAT_EQ(Variance(data), 5.04);
}

TEST(Analyze, StatsEmpty) {
    Series data{};
    ASSERT_TRUE(std::isnan(Mean(data)));
    ASSERT_TRUE(std::isnan(Variance(data)));

    Series data2{5};
    EXPECT_FLOAT_EQ(Mean(data2), 5);
    EXPECT_FLOAT_EQ(Variance(data2), 0);
}