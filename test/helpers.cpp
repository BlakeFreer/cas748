#include "gtest/gtest.h"
#include "helpers.hpp"

void CompareSeries(const cas::Series& x, const cas::Series& y) {
    ASSERT_EQ(x.size(), y.size());

    for (int i = 0; i < y.size(); i++) {
        EXPECT_FLOAT_EQ(x[i], y[i]);
    }
}