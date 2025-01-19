#include "caslib/analyze.hpp"
#include "caslib/series.hpp"
#include "gtest/gtest.h"
#include "helpers.hpp"

using namespace cas;

TEST(Analyze, CrossCorrelation) {
    Series x{1, 1, 1, 1, 1};
    Series y{1, 2, 3, 4, 5};

    Series cc = CrossCorrelation(x, y);
    Series expected{5, 9, 12, 14, 15, 10, 6, 3, 1};

    CompareSeries(cc, expected);
}