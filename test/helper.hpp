#pragma once

#include <complex>

#include "Eigen/Dense"
#include "gtest/gtest.h"

#define EXPECT_ARRAY_NEAR(x, y, tol)                                 \
    ASSERT_EQ((x).size(), (y).size());                               \
    for (int i = 0; i < (x).size(); i++) {                           \
        EXPECT_NEAR((x)[i], (y)[i], tol) << "differ at index " << i; \
    }

#define EXPECT_ARRAY_EQ(x, y) EXPECT_ARRAY_NEAR((x), (y), 1e-4);

#define _CCd(x) std::complex<double>(x)
#define EXPECT_COMPLEXD_EQ(x, y)                      \
    EXPECT_DOUBLE_EQ(_CCd(x).real(), _CCd(y).real()); \
    EXPECT_DOUBLE_EQ(_CCd(x).imag(), _CCd(y).imag());
