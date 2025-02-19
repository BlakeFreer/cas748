#include <Eigen/Core>

#include "caslib/csv.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(IO, LoadCSV_Single) {
    Eigen::ArrayXd loaded = LoadCSV("data/test_load_series.csv").col(0);
    Eigen::ArrayXd expected{{100, 29.03, 0.83394, 48.1, -299.2, 8, -100}};

    ASSERT_EQ(loaded.rows(), expected.rows());
    ASSERT_EQ(loaded.cols(), expected.cols());

    EXPECT_TRUE(loaded.isApprox(expected));
}

TEST(IO, LoadCSV) {
    Eigen::ArrayXXd table = LoadCSV("data/test_load_dataset.csv");

    ASSERT_EQ(table.cols(), 4);
    ASSERT_EQ(table.rows(), 6);
    Eigen::ArrayXXd expected(6, 4);

    // clang-format off
    expected << 10, 20, 30, 40,
                11, 21, 31, 41,
                12, 22, 32, 42,
                13, 23, 33, 43,
                14, 24, 34, 44,
                15, 25, 35, 45;
    // clang-format on

    EXPECT_TRUE(table.isApprox(expected));
}