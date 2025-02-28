#include "caslib/savitzky_golay.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(Filter, SavitzkyGolay) {
    // Expected filter values from:
    // https://en.wikipedia.org/wiki/Savitzky%E2%80%93Golay_filter#Tables_of_selected_convolution_coefficients

    Eigen::ArrayXd expected;

    expected = Eigen::ArrayXd{{-3, 12, 17, 12, -3}} / 35;
    EXPECT_TRUE(filter::SavitzkyGolay(5, 2).isApprox(expected));
    EXPECT_TRUE(filter::SavitzkyGolay(5, 3).isApprox(expected));

    expected = Eigen::ArrayXd{{-2, 3, 6, 7, 6, 3, -2}} / 21;
    EXPECT_TRUE(filter::SavitzkyGolay(7, 2).isApprox(expected));
    EXPECT_TRUE(filter::SavitzkyGolay(7, 3).isApprox(expected));

    expected = Eigen::ArrayXd{{-21, 14, 39, 54, 59, 54, 39, 14, -21}} / 231;
    EXPECT_TRUE(filter::SavitzkyGolay(9, 2).isApprox(expected));
    EXPECT_TRUE(filter::SavitzkyGolay(9, 3).isApprox(expected));

    expected = Eigen::ArrayXd{{5, -30, 75, 131, 75, -30, 5}} / 231;
    EXPECT_TRUE(filter::SavitzkyGolay(7, 4).isApprox(expected));
    EXPECT_TRUE(filter::SavitzkyGolay(7, 5).isApprox(expected));

    expected = Eigen::ArrayXd{{15, -55, 30, 135, 179, 135, 30, -55, 15}} / 429;
    EXPECT_TRUE(filter::SavitzkyGolay(9, 4).isApprox(expected));
    EXPECT_TRUE(filter::SavitzkyGolay(9, 5).isApprox(expected));
}
