#include "caslib/analyze.hpp"

#include <cassert>

#include "caslib/series.hpp"

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
            overlap = x.SubSeries(0, lag) * y.SubSeries(-lag, y.ssize());
        } else if (lag == 0) {
            overlap = x * y;
        } else {  // y is delayed
            overlap = x.SubSeries(lag, x.ssize()) * y.SubSeries(0, -lag);
        }
        corel[lag + x.ssize() - 1] = overlap.sum();
    }

    return corel;
}
}  // namespace cas