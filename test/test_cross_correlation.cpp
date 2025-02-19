#include "caslib/analyze.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(Analyze, CrossCorrelation) {
    Eigen::ArrayXd x{{1, 1, 1, 1, 1}};
    Eigen::ArrayXd y{{1, 2, 3, 4, 5}};

    Eigen::ArrayXd cc = CrossCorrelation(x, y);
    Eigen::ArrayXd expected{{5, 9, 12, 14, 15, 10, 6, 3, 1}};

    EXPECT_TRUE(cc.isApprox(expected));
}