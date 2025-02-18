#include <fftw3.h>

#include <Eigen/Dense>
#include <cassert>
#include <complex>
#include <iostream>

inline fftw_complex* to_fftw(Eigen::VectorXcd& v) {
    return reinterpret_cast<fftw_complex*>(v.data());
}

int main(int argc, char* argv[]) {
    int N = argc - 1;
    Eigen::VectorXcd z(N);

    for (int i = 0; i < N; i++) {
        z(i) = {std::stod(argv[i + 1]), 0};
    }

    std::cout << z << std::endl;

    Eigen::VectorXcd zh(N);

    fftw_plan p = fftw_plan_dft_1d(N, to_fftw(z), to_fftw(zh), FFTW_FORWARD,
                                   FFTW_ESTIMATE);
    fftw_execute(p);
    zh /= N;

    for (int i = 0; i < N; i++) {
        std::cout << "i: " << i << " o: " << zh[i] << std::endl;
    }

    Eigen::VectorXcd z2(N);

    fftw_plan p2 = fftw_plan_dft_1d(N, to_fftw(zh), to_fftw(z2), FFTW_BACKWARD,
                                    FFTW_ESTIMATE);
    fftw_execute(p2);

    for (int i = 0; i < N; i++) {
        std::cout << z2[i] << ", ";
    }
    fftw_free(p);
    fftw_free(p2);

    return 0;
}