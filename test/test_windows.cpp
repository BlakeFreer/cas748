#include "caslib/series.hpp"
#include "caslib/windows.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(Windows, Bartlett) {
    // Expected values from
    // https://numpy.org/doc/stable/reference/generated/numpy.bartlett.html

    Series w = window::Bartlett(12);

    EXPECT_EQ(w.size(), 12);

    EXPECT_FLOAT_EQ(w[0], 0);
    EXPECT_FLOAT_EQ(w[1], 0.18181818);
    EXPECT_FLOAT_EQ(w[2], 0.36363636);
    EXPECT_FLOAT_EQ(w[3], 0.54545455);
    EXPECT_FLOAT_EQ(w[4], 0.72727273);
    EXPECT_FLOAT_EQ(w[5], 0.90909091);
    EXPECT_FLOAT_EQ(w[6], 0.90909091);
    EXPECT_FLOAT_EQ(w[7], 0.72727273);
    EXPECT_FLOAT_EQ(w[8], 0.54545455);
    EXPECT_FLOAT_EQ(w[9], 0.36363636);
    EXPECT_FLOAT_EQ(w[10], 0.18181818);
    EXPECT_FLOAT_EQ(w[11], 0);
}