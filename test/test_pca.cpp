#include <math.h>

#include <Eigen/Core>

#include "caslib/analyze.hpp"
#include "gtest/gtest.h"
#include "helper.hpp"

TEST(PCA, Exact) {
    Eigen::ArrayXXd data{{
        {2, 2},
        {-2, -2},
        {1, -1},
        {-1, 1},
    }};
    Eigen::ArrayXXd x = cas::PrincipleComponentAnalysis(data);
    Eigen::ArrayXXd expected_evec{{
        {1, 1},
        {-1, 1},
    }};
    Eigen::ArrayXXd expected = expected_evec.transpose() / sqrt(2);

    EXPECT_ARRAY_EQ(x.col(0), expected.col(0));
    EXPECT_ARRAY_EQ(x.col(1), expected.col(1));
}