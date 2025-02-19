// Expected values from Numpy docs, such as
// https://numpy.org/doc/stable/reference/generated/numpy.bartlett.html

#include <Eigen/Core>

#include "caslib/windows.hpp"
#include "gtest/gtest.h"

using namespace cas;

class WindowsTest : public ::testing::Test {
protected:
    void TestWindow(const Eigen::ArrayXd& window,
                    const Eigen::ArrayXd& expected) {
        EXPECT_EQ(window.size(), expected.size());
        EXPECT_LT((window - expected).abs().maxCoeff(), 0.001);
    }
};

TEST_F(WindowsTest, Hanning) {
    Eigen::ArrayXd expected{{0., 0.07937323, 0.29229249, 0.57115742, 0.82743037,
                             0.97974649, 0.97974649, 0.82743037, 0.57115742,
                             0.29229249, 0.07937323, 0.}};
    TestWindow(window::Hanning<double>(12), expected);
}

TEST_F(WindowsTest, Hamming) {
    Eigen::ArrayXd expected{{0.08, 0.15302337, 0.34890909, 0.60546483,
                             0.84123594, 0.98136677, 0.98136677, 0.84123594,
                             0.60546483, 0.34890909, 0.15302337, 0.08}};
    TestWindow(window::Hamming<double>(12), expected);
}

TEST_F(WindowsTest, Blackman) {
    Eigen::ArrayXd expected{{-1.38777878e-17, 3.26064346e-02, 1.59903635e-01,
                             4.14397981e-01, 7.36045180e-01, 9.67046769e-01,
                             9.67046769e-01, 7.36045180e-01, 4.14397981e-01,
                             1.59903635e-01, 3.26064346e-02, -1.38777878e-17}};
    TestWindow(window::Blackman<double>(12), expected);
}

TEST_F(WindowsTest, Bartlett) {
    Eigen::ArrayXd expected{{0., 2., 4., 6., 8., 10., 10., 8., 6., 4., 2., 0.}};
    expected /= 11;
    TestWindow(window::Bartlett<double>(12), expected);
}

TEST_F(WindowsTest, Rectangular) {
    TestWindow(window::Rectangular<double>(12),
               Eigen::ArrayXd::Constant(12, 1. / 12.));
}
