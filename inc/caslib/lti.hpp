#pragma once
#include <array>

namespace cas {

/// @brief SISO LTI Filter
///
/// @tparam N size of input memory (incl. x[n])
/// @tparam M size of output memory (excl. y[n])
///  y[n] =  a[0]x[n]   + a[1]x[n-1] + a[2]x[n-2] + ... to N
//         + b[0]y[n-1] + b[1]y[n-2] +              ... to M
template <int N, int M, typename T = double>
class LtiFilter {
public:
    LtiFilter(std::array<T, N> ak, std::array<T, M> bk);

    T Update(T x);
    T GetOutput() const;

private:
    std::array<T, N> ak_;
    std::array<T, N> xn_;

    std::array<T, M> bk_;
    std::array<T, M + 1> yn_;
};

template <int N, int M, typename T>
LtiFilter<N, M, T>::LtiFilter(std::array<T, N> ak, std::array<T, M> bk)
    : ak_(ak), bk_(bk) {
    xn_.fill(0);
    yn_.fill(0);
}

template <int N, int M, typename T>
T LtiFilter<N, M, T>::Update(T x) {
    // shift input memory (not efficient, should use circular buffer)
    for (int i = N - 1; i > 0; i--) xn_[i] = xn_[i - 1];
    for (int i = M; i > 0; i--) yn_[i] = yn_[i - 1];
    xn_[0] = x;
    yn_[0] = 0;

    // compute new output
    for (int i = 0; i < N; i++) yn_[0] += ak_[i] * xn_[i];
    for (int i = 0; i < M; i++) yn_[0] += bk_[i] * yn_[i + 1];

    return GetOutput();
}

template <int N, int M, typename T>
T LtiFilter<N, M, T>::GetOutput() const {
    return yn_[0];
}

}  // namespace cas