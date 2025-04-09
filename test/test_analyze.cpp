#include <Eigen/Core>

#include "caslib/analyze.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(Analyze, LinearFit) {
    Eigen::ArrayXd x = Eigen::ArrayXd::LinSpaced(100, 0, 99);

    Eigen::ArrayXXd xy(100, 2);
    xy.col(0) = x;
    xy.col(1) = 2 * x * x - 3 * x + 5;

    Eigen::ArrayXd coeff = PolynomialFit(2, xy);
    EXPECT_NEAR(coeff[2], 2, 0.001);
    EXPECT_NEAR(coeff[1], -3, 0.001);
    EXPECT_NEAR(coeff[0], 5, 0.001);
}