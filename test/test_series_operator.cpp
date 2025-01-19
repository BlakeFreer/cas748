#include "caslib/series.hpp"
#include "gtest/gtest.h"
#include "helpers.hpp"

using namespace cas;

TEST(SeriesOperator, add) {
    Series x{1, 2, 3};
    Series y{50, 100, 200};

    CompareSeries(x + y, {51, 102, 203});
}

TEST(SeriesOperator, divide) {
    Series x{1, 2, 3};
    CompareSeries(x / 10, {0.1, 0.2, 0.3});
}

TEST(SeriesOperator, multiply) {
    Series x{1, 2, 3};
    float scalar = -0.5;
    Series expected{-0.5, -1.0, -1.5};

    CompareSeries(-0.5 * x, expected);
    CompareSeries(x * -0.5, expected);
}

TEST(SeriesOperator, negative) {
    Series x{1, 2, 3};
    CompareSeries(-x, {-1, -2, -3});
}