#pragma once

#include <Eigen/Core>

namespace cas {

template <typename Derived>
typename Eigen::ScalarBinaryOpTraits<typename Derived::Scalar,
                                     typename Derived::Scalar>::ReturnType
Correlation(const Eigen::ArrayBase<Derived>& x,
            const Eigen::ArrayBase<Derived>& y) {
    return (x * y.conjugate()).sum();
}

template <typename Derived>
typename Eigen::ScalarBinaryOpTraits<typename Derived::Scalar,
                                     typename Derived::Scalar>::ReturnType
Covariance(const Eigen::ArrayBase<Derived>& x,
           const Eigen::ArrayBase<Derived>& y) {
    return Correlation(x - x.mean(), y - y.mean()) / x.size();
}

template <typename Derived>
typename Eigen::ScalarBinaryOpTraits<typename Derived::Scalar,
                                     typename Derived::Scalar>::ReturnType
Variance(const Eigen::ArrayBase<Derived>& x) {
    return Covariance(x, x);
}

/// @brief Calculate the Cross Correlation of two Series at all time lags.
/// @details Slides `x`, starting with the `x[0]` aligned with `y[last]` until
/// `x[last]` is aligned with `y[0]`.
///
/// @param x
/// @param y
template <typename Derived>
Derived CrossCorrelation(const Eigen::ArrayBase<Derived>& x,
                         const Eigen::ArrayBase<Derived>& y) {
    Derived corel(x.size() * 2 - 1);

    for (int lag = -x.size() + 1; lag <= x.size() - 1; lag++) {
        if (lag < 0) {  // x is delayed
            corel[lag + x.size() - 1] =
                Correlation(x(Eigen::seq(0, Eigen::last + lag)),
                            y(Eigen::seq(-lag, Eigen::last)));
        } else if (lag == 0) {
            corel[lag + x.size() - 1] = Correlation(x, y);
        } else {  // y is delayed
            corel[lag + x.size() - 1] =
                Correlation(x(Eigen::seq(lag, Eigen::last)),
                            y(Eigen::seq(0, Eigen::last - lag)));
        }
    }

    return corel;
}

}  // namespace cas