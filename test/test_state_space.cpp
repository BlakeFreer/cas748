#include <Eigen/Core>

#include "caslib/state_space.hpp"
#include "gtest/gtest.h"
#include "helper.hpp"

using namespace cas;

TEST(StateSpace, Accelerate) {
    Eigen::Matrix<double, 2, 2> A{
        {1, 1},
        {0, 1},
    };
    Eigen::Matrix<double, 2, 1> B{{0, 1}};
    Eigen::Matrix<double, 1, 2> C{{1, 0}};

    StateSpace<2, 1, 1> sys{A, B, C};

    EXPECT_ARRAY_EQ(sys.GetState(), Eigen::VectorXd::Zero(2));

    for (int i = 1; i <= 10; i++) {
        sys.Update(Eigen::Vector<double, 1>(1.f));
        EXPECT_DOUBLE_EQ(sys.GetState()(1), i);
        EXPECT_DOUBLE_EQ(sys.GetOutput()(0), (double)i * (i - 1) / 2.);
    }
}