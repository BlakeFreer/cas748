#include <vector>

#include "caslib/io.hpp"
#include "caslib/series.hpp"
#include "gtest/gtest.h"
#include "helpers.hpp"

using namespace cas;

TEST(IO, LoadCSV) {
    Series loaded = Load("data/test_load_series.csv");
    Series expected{100, 29.03, 0.83394, 48.1, -299.2, 8, -100};

    CompareSeries(loaded, expected);
}

TEST(IO, LoadCSVCols) {
    std::vector<Series> columns = LoadSeries("data/test_load_dataset.csv");

    ASSERT_EQ(columns.size(), 4);
    CompareSeries(columns[0], {10, 11, 12, 13, 14, 15});
    CompareSeries(columns[1], {20, 21, 22, 23, 24, 25});
    CompareSeries(columns[2], {30, 31, 32, 33, 34, 35});
    CompareSeries(columns[3], {40, 41, 42, 43, 44, 45});
}