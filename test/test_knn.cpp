#include <Eigen/Core>

#include "caslib/analyze.hpp"
#include "gtest/gtest.h"

TEST(KNN, k3) {
    Eigen::ArrayXXd points{{{-4, -3, -2, -1, 0, 1, 2, 3, 4}}};
    points.transposeInPlace();
    Eigen::ArrayXi labels{{-1, -1, -1, 1, 1, 1, -1, -1, -1}};

    for (double d = -10; d < 10; d += 0.01) {
        int expected = (d > -1.5 && d < 1.5) ? 1 : -1;
        ASSERT_EQ(expected, cas::KNearestNeighbours(points, labels,
                                                    Eigen::ArrayXd{{d}}, 3))
            << "Failed on " << d;
    }
}