// Expected values from Numpy docs, such as
// https://numpy.org/doc/stable/reference/generated/numpy.bartlett.html

#include <Eigen/Core>

#include "caslib/windows.hpp"
#include "gtest/gtest.h"
#include "helper.hpp"

using namespace cas;

TEST(Windows, Hanning) {
    Eigen::ArrayXd expected{{0., 0.07937323, 0.29229249, 0.57115742, 0.82743037,
                             0.97974649, 0.97974649, 0.82743037, 0.57115742,
                             0.29229249, 0.07937323, 0.}};
    EXPECT_ARRAY_EQ(window::Hanning<double>(12), expected);
}

TEST(Windows, Hamming) {
    Eigen::ArrayXd expected{{0.08, 0.15302337, 0.34890909, 0.60546483,
                             0.84123594, 0.98136677, 0.98136677, 0.84123594,
                             0.60546483, 0.34890909, 0.15302337, 0.08}};
    EXPECT_ARRAY_EQ(window::Hamming<double>(12), expected);
}

TEST(Windows, Blackman) {
    Eigen::ArrayXd expected{{-1.38777878e-17, 3.26064346e-02, 1.59903635e-01,
                             4.14397981e-01, 7.36045180e-01, 9.67046769e-01,
                             9.67046769e-01, 7.36045180e-01, 4.14397981e-01,
                             1.59903635e-01, 3.26064346e-02, -1.38777878e-17}};
    EXPECT_ARRAY_EQ(window::Blackman<double>(12), expected);
}

TEST(Windows, Bartlett) {
    Eigen::ArrayXd expected{{0., 2., 4., 6., 8., 10., 10., 8., 6., 4., 2., 0.}};
    expected /= 11;
    EXPECT_ARRAY_EQ(window::Bartlett<double>(12), expected);
}

TEST(Windows, Rectangular) {
    EXPECT_ARRAY_EQ(window::Rectangular<double>(12),
                    Eigen::ArrayXd::Constant(12, 1. / 12.));
}
