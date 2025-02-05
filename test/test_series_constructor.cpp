#include "caslib/series.hpp"
#include "gtest/gtest.h"
#include "helpers.hpp"

using namespace cas;

TEST(SeriesConstructor, Copy) {
    Series x{1, 2, 3};
    Series y{x};  // should call copy constructor

    CompareSeries(x, y);
}

TEST(SeriesConstructor, Move) {
    Series x{1, 2, 3};
    Series y{std::move(x)};

    EXPECT_EQ(x.size(), 0);
    CompareSeries(y, {1, 2, 3});
}