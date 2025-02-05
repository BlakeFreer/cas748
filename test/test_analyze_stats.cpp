#include <cmath>

#include "caslib/series.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(Analyze, Stats) {
    Series data{3, 0, 4, -2, 3};
    EXPECT_FLOAT_EQ(data.sum(), 8);
    EXPECT_FLOAT_EQ(data.mean(), 1.6);
    EXPECT_FLOAT_EQ(data.variance(), 5.04);
}

TEST(Analyze, StatsEmpty) {
    Series data{};
    EXPECT_FLOAT_EQ(data.sum(), 0);
    EXPECT_TRUE(std::isnan(data.mean()));
    EXPECT_TRUE(std::isnan(data.variance()));

    Series data2{5};
    EXPECT_FLOAT_EQ(data.sum(), 0);
    EXPECT_FLOAT_EQ(data2.mean(), 5);
    EXPECT_FLOAT_EQ(data2.variance(), 0);
}

TEST(Analyze, Norm) {
    Series x{3, 4};
    EXPECT_FLOAT_EQ(x.norm(1), 7);

    EXPECT_FLOAT_EQ(x.norm(2), 5);
    EXPECT_FLOAT_EQ(x.norm_to_p(2), 25);

    EXPECT_FLOAT_EQ(x.norm_to_p(3), 91);
}