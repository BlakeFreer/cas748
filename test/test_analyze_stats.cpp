#include <Eigen/Core>
#include <cmath>

#include "caslib/analyze.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(Analyze, Stats) {
    Eigen::ArrayXd data{{3, 0, 4, -2, 3}};
    EXPECT_FLOAT_EQ(data.sum(), 8);
    EXPECT_FLOAT_EQ(data.mean(), 1.6);
    EXPECT_FLOAT_EQ(Variance(data), 5.04);
}

TEST(Analyze, StatsEmpty) {
    Eigen::ArrayXd data2{{5}};
    EXPECT_FLOAT_EQ(data2.sum(), 5);
    EXPECT_FLOAT_EQ(data2.mean(), 5);
    EXPECT_FLOAT_EQ(Variance(data2), 0);
}

TEST(Analyze, Norm) {
    Eigen::ArrayXd x{{3, 4}};
    EXPECT_FLOAT_EQ(x.matrix().lpNorm<1>(), 7);

    EXPECT_FLOAT_EQ(x.matrix().lpNorm<2>(), 5);

    EXPECT_FLOAT_EQ(x.matrix().lpNorm<3>(), 4.49794144527541479639);
}