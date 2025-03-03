#pragma once

#include "Eigen/Dense"
#include "gtest/gtest.h"

#define EXPECT_ARRAY_NEAR(x, y, tol)                                 \
    ASSERT_EQ((x).size(), (y).size());                               \
    for (int i = 0; i < (x).size(); i++) {                           \
        EXPECT_NEAR((x)[i], (y)[i], tol) << "differ at index " << i; \
    }

#define EXPECT_ARRAY_EQ(x, y) EXPECT_ARRAY_NEAR((x), (y), 1e-4);