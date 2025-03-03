#include "caslib/analyze.hpp"
#include "caslib/savitzky_golay.hpp"
#include "gtest/gtest.h"
#include "helper.hpp"

using namespace cas;

TEST(Filter, Pad) {
    Eigen::ArrayXd in{{1, 2, 3, 4, 5}};

    Eigen::ArrayXd pad3 = Pad(in, 3);
    Eigen::ArrayXd exp3{{0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0}};
    EXPECT_ARRAY_EQ(pad3, exp3);

    Eigen::ArrayXd pad0 = Pad(in, 0);
    EXPECT_ARRAY_EQ(pad0, in);

    Eigen::ArrayXd pad1 = Pad(in, 1);
    Eigen::ArrayXd exp1{{0, 1, 2, 3, 4, 5, 0}};
    EXPECT_ARRAY_EQ(pad1, exp1);
}

TEST(Filter, Kronecker) {
    // Filter with Kronecker -> should return same thing
    Eigen::ArrayXd in{{1, 2, 3, 4, 5, 6}};
    Eigen::ArrayXd filter{{1}};

    EXPECT_ARRAY_EQ(cas::ApplyFilter(in, filter), in);
}

TEST(Filter, Lpf2) {
    Eigen::ArrayXd in{{0, 1, -1, 0, 2, 0}};
    Eigen::ArrayXd filt{{0.5, 0.5}};

    Eigen::ArrayXd exp{{0, 0.5, 0, -0.5, 1, 1, 0}};
    EXPECT_ARRAY_EQ(cas::ApplyFilter(in, filt), exp);
}

TEST(Filter, Hpf2) {
    Eigen::ArrayXd in{{-1, 1, -1, 1}};
    Eigen::ArrayXd filt{{0.5, -0.5}};

    Eigen::ArrayXd exp{{-0.5, 1, -1, 1, -0.5}};
    EXPECT_ARRAY_EQ(cas::ApplyFilter(in, filt), exp);
}

TEST(Filter, Triangle) {
    Eigen::ArrayXd in{{0, 1, 4, 5, 2}};
    Eigen::ArrayXd rect_filt{{1, 1, 1}};
    Eigen::ArrayXd tri_filt = cas::ApplyFilter(rect_filt, rect_filt);

    Eigen::ArrayXd exp{{1, 2, 3, 2, 1}};
    EXPECT_ARRAY_EQ(tri_filt, exp);
}

TEST(Filter, SavitzkyGolay) {
    // Expected filter values from:
    // https://en.wikipedia.org/wiki/Savitzky%E2%80%93Golay_filter#Tables_of_selected_convolution_coefficients

    Eigen::ArrayXd expected;

    expected = Eigen::ArrayXd{{-3, 12, 17, 12, -3}} / 35;
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(5, 2), expected);
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(5, 3), expected);

    expected = Eigen::ArrayXd{{-2, 3, 6, 7, 6, 3, -2}} / 21;
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(7, 2), expected);
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(7, 3), expected);

    expected = Eigen::ArrayXd{{-21, 14, 39, 54, 59, 54, 39, 14, -21}} / 231;
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(9, 2), expected);
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(9, 3), expected);

    expected = Eigen::ArrayXd{{5, -30, 75, 131, 75, -30, 5}} / 231;
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(7, 4), expected);
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(7, 5), expected);

    expected = Eigen::ArrayXd{{15, -55, 30, 135, 179, 135, 30, -55, 15}} / 429;
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(9, 4), expected);
    EXPECT_ARRAY_EQ(filter::SavitzkyGolay(9, 5), expected);
}
