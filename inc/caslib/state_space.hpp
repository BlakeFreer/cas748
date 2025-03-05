#pragma once

#include <Eigen/Core>

namespace cas {

/// @brief Represents a state-space system
///
/// @tparam Sx size of state vector
/// @tparam Su size of input vector
/// @tparam Sy size of output vector
template <int Sx, int Su, int Sy>
class StateSpace {
    template <int R, int C>
    using Matrix = Eigen::Matrix<double, R, C>;

    template <int N>
    using Vector = Eigen::Vector<double, N>;

public:
    StateSpace(Matrix<Sx, Sx> A, Matrix<Sx, Su> B, Matrix<Sy, Sx> C);
    StateSpace(Matrix<Sx, Sx> A, Matrix<Sx, Su> B, Matrix<Sy, Sx> C,
               Vector<Sx> X0);

    Vector<Sx> GetState() const;
    Vector<Sy> GetOutput() const;
    void Update(Vector<Su> u);

private:
    Matrix<Sx, Sx> A_;
    Matrix<Sx, Su> B_;
    Matrix<Sy, Sx> C_;
    Vector<Sx> X_;
};

template <int Sx, int Su, int Sy>
StateSpace<Sx, Su, Sy>::StateSpace(Matrix<Sx, Sx> A, Matrix<Sx, Su> B,
                                   Matrix<Sy, Sx> C)
    : StateSpace(A, B, C, Vector<Sx>::Zero()) {}

template <int Sx, int Su, int Sy>
StateSpace<Sx, Su, Sy>::StateSpace(Matrix<Sx, Sx> A, Matrix<Sx, Su> B,
                                   Matrix<Sy, Sx> C, Vector<Sx> X0)
    : A_(A), B_(B), C_(C), X_(X0) {}

template <int Sx, int Su, int Sy>
Eigen::Vector<double, Sx> StateSpace<Sx, Su, Sy>::GetState() const {
    return X_;
}

template <int Sx, int Su, int Sy>
Eigen::Vector<double, Sy> StateSpace<Sx, Su, Sy>::GetOutput() const {
    return C_ * X_;
}

template <int Sx, int Su, int Sy>
void StateSpace<Sx, Su, Sy>::Update(Vector<Su> u) {
    X_ = A_ * X_ + B_ * u;
}

}  // namespace cas