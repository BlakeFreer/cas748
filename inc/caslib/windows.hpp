// Equations from
// https://numpy.org/doc/stable/reference/routines.window.html

#pragma once

#include <Eigen/Core>
#include <cmath>

namespace cas::window {

template <typename T>
Eigen::ArrayX<T> Hanning(int N) {
    const float omega = 2 * std::numbers::pi / (N - 1);
    return Eigen::ArrayX<T>::LinSpaced(N, 0, N - 1).unaryExpr([&](T n) {
        return 0.5 - 0.5 * std::cos(omega * n);
    });
}

template <typename T>
Eigen::ArrayX<T> Hamming(int N) {
    const float omega = 2 * std::numbers::pi / (N - 1);
    return Eigen::ArrayX<T>::LinSpaced(N, 0, N - 1).unaryExpr([&](T n) {
        return 0.54 - 0.46 * std::cos(omega * n);
    });
}

template <typename T>
Eigen::ArrayX<T> Blackman(int N) {
    return Eigen::ArrayX<T>::LinSpaced(N, 0, N - 1).unaryExpr([&](T n) {
        return 0.42 - 0.5 * std::cos(2. * std::numbers::pi * n / (N - 1)) +
               0.08 * std::cos(4. * std::numbers::pi * n / (N - 1));
    });
}

template <typename T>
Eigen::ArrayX<T> Bartlett(int N) {
    const float x = (N - 1) / 2.f;
    return Eigen::ArrayX<T>::LinSpaced(N, 0, N - 1).unaryExpr([&](T n) {
        return (x - std::abs(n - x)) / x;
    });
}

template <typename T>
Eigen::ArrayX<T> Rectangular(int N) {
    return Eigen::ArrayX<T>::Ones(N) / N;
}

}  // namespace cas::window