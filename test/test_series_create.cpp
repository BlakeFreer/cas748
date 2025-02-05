#include "caslib/series.hpp"
#include "gtest/gtest.h"
#include "helpers.hpp"

using namespace cas;

TEST(SeriesCreate, Zeros) {
    auto x = Series::Zeros(100);

    for (auto f : x) {
        ASSERT_FLOAT_EQ(f, 0);
    }
}

TEST(SeriesCreate, Ones) {
    auto x = Series::Ones(100);

    for (auto f : x) {
        ASSERT_FLOAT_EQ(f, 1);
    }
}

TEST(SeriesCreate, InitializerList) {
    auto x = Series{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < 10; i++) {
        ASSERT_FLOAT_EQ(x[i], i);
    }
}

TEST(SeriesCreate, Vector) {
    std::vector<float> x = {-10, 0, 100};
    Series s{x};

    ASSERT_FLOAT_EQ(s[0], -10);
    ASSERT_FLOAT_EQ(s[1], 0);
    ASSERT_FLOAT_EQ(s[2], 100);
}

TEST(SeriesCreate, VectorEmpty) {
    std::vector<float> x;
    Series s{x};

    ASSERT_EQ(s.size(), 0);
}

TEST(SeriesCreate, Apply) {
    Series s{1, 2, 3, 4, 5};
    auto x = s.Apply([](float f) { return f * f; });

    CompareSeries(x, {1, 4, 9, 16, 25});
}