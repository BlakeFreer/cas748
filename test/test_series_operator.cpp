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
    Series y{5, 6, 7};

    CompareSeries(x * y, {5, 12, 21});
}

TEST(SeriesOperator, negative) {
    Series x{1, 2, 3};
    CompareSeries(-x, {-1, -2, -3});
}

TEST(SeriesOperator, sub_scalar) {
    Series x{1, 2, 3};
    Series expected{-9, -8, -7};

    CompareSeries(x - 10, expected);
    CompareSeries(10 - x, -expected);
}

TEST(SeriesOperator, mult_scalar) {
    Series x{1, 2, 3};
    Series expected{-0.5, -1.0, -1.5};

    CompareSeries(-0.5 * x, expected);
    CompareSeries(x * -0.5, expected);
}

TEST(SeriesOperator, add_scalar) {
    Series x{1, 2, 3};
    CompareSeries(x + 10, {11, 12, 13});
    CompareSeries(10 + x, {11, 12, 13});
}
