#include <Eigen/Core>
#include <complex>

#include "caslib/lti.hpp"
#include "gtest/gtest.h"
#include "helper.hpp"

using namespace cas;

TEST(LtiFilter, Avg2) {
    LtiFilter<2, 0> lti({0.5, 0.5}, {});
    EXPECT_EQ(lti.GetOutput(), 0);

    Eigen::ArrayXd input{{1, 1, 3, 0}};
    Eigen::ArrayXd exp{{0.5, 1, 2, 1.5}};

    Eigen::ArrayXd out = input.unaryExpr([&](auto d) { return lti.Update(d); });
    EXPECT_ARRAY_EQ(out, exp);
}

TEST(LtiFilter, Hpf2) {
    LtiFilter<2, 0> lti({0.5, -0.5}, {});

    Eigen::ArrayXd input{{1, 1, 3, 0}};
    Eigen::ArrayXd exp{{0.5, 0, 1, -1.5}};

    Eigen::ArrayXd out = input.unaryExpr([&](auto d) { return lti.Update(d); });
    EXPECT_ARRAY_EQ(out, exp);
}

TEST(LtiFilter, Exponential) {
    LtiFilter<1, 1> lti({1}, {0.5});

    Eigen::ArrayXd input{{8, 0, 0, 15, 0, 0}};
    Eigen::ArrayXd exp{{8, 4, 2, 16, 8, 4}};

    Eigen::ArrayXd out = input.unaryExpr([&](auto d) { return lti.Update(d); });
    EXPECT_ARRAY_EQ(out, exp);
}

TEST(LtiFilter, FeedbackLowPass) {
    LtiFilter<1, 1> lti({0.5}, {0.5});

    Eigen::ArrayXd input{{16, 0, 0, 0}};
    Eigen::ArrayXd exp{{8, 4, 2, 1}};

    Eigen::ArrayXd out = input.unaryExpr([&](auto d) { return lti.Update(d); });
    EXPECT_ARRAY_EQ(out, exp);
}

TEST(LtiFilter, Fourier) {
    // Calculate the fourier coefficients using a filter
    Eigen::ArrayXd signal{{4, 1, 0, -1}};

    using complex = std::complex<double>;
    using Filter = LtiFilter<4, 0, complex>;
    const complex i{0, 1};

    // filters defined in reverse since they are reversed by LTI relative to
    // signal
    std::array<Filter, 4> filters = {
        Filter{{1, 1, 1, 1}, {}},
        Filter{{i, -1, -i, 1}, {}},
        Filter{{-1, 1, -1, 1}, {}},
        Filter{{-i, -1, i, 1}, {}},
    };

    std::array<complex, 4> Xk;
    for (int k = 0; k < 4; k++) {
        for (int n = 0; n < 4; n++) {
            filters[k].Update(signal[n]);
        }
        Xk[k] = filters[k].GetOutput() / 4.;
    }

    EXPECT_COMPLEXD_EQ(Xk[0], 1);
    EXPECT_COMPLEXD_EQ(Xk[1], 1. - 0.5 * i);
    EXPECT_COMPLEXD_EQ(Xk[2], 1);
    EXPECT_COMPLEXD_EQ(Xk[3], 1. + 0.5 * i);
}