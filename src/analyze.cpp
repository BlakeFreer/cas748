#include "caslib/analyze.hpp"

#include <cassert>
#include <cmath>

#include "caslib/series.hpp"
#include "fftw3.h"

namespace cas {

Series CrossCovariance(const Series& x, const Series& y) {
    return CrossCorrelation(x - x.mean(), y - y.mean());
}

Series CrossCorrelation(const Series& x, const Series& y) {
    assert(x.size() == y.size());

    auto corel = Series::Zeros(x.size() * 2 - 1);

    for (int lag = -x.ssize() + 1; lag <= x.ssize() - 1; lag++) {
        Series overlap;
        if (lag < 0) {  // x is delayed
            overlap = x.View(0, lag) * y.View(-lag, y.ssize());
        } else if (lag == 0) {
            overlap = x * y;
        } else {  // y is delayed
            overlap = x.View(lag, x.ssize()) * y.View(0, -lag);
        }
        corel[lag + x.ssize() - 1] = overlap.sum();
    }

    return corel;
}

Series FourierTransform(const Series& x) {
    // copy the data
    int N = x.size();

    double* data = fftw_alloc_real(N);
    for (int i = 0; i < N; i++) {
        data[i] = static_cast<double>(x[i]);
    }

    int N_OUT = N / 2 + 1;
    fftw_complex* out = fftw_alloc_complex(N_OUT);

    fftw_plan plan;
    plan = fftw_plan_dft_r2c_1d(N, data, out, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_free(data);

    // Copy into a series
    Series dft(N_OUT);
    for (int i = 0; i < N_OUT; i++) {
        float a = out[i][0];
        float b = out[i][1];
        dft[i] = std::sqrt(a * a + b * b);
    }
    return dft;
}

}  // namespace cas