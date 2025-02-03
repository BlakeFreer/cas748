#include "helpers.hpp"

#include "gtest/gtest.h"

void CompareSeries(const cas::Series& x, const cas::Series& y) {
    ASSERT_EQ(x.size(), y.size());

    for (int i = 0; i < y.size(); i++) {
        EXPECT_FLOAT_EQ(x[i], y[i]);
    }
}