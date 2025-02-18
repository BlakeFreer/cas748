#include "caslib/series.hpp"
#include "gtest/gtest.h"
#include "helpers.hpp"

using namespace cas;

TEST(SeriesView, Strides) {
    Series s{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    CompareSeries(s.View(0, s.ssize(), 1), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    CompareSeries(s.View(0, s.ssize(), 2), {0, 2, 4, 6, 8});
    CompareSeries(s.View(0, s.ssize(), 3), {0, 3, 6, 9});
    CompareSeries(s.View(0, s.ssize(), 4), {0, 4, 8});
    CompareSeries(s.View(0, s.ssize(), 5), {0, 5});
    CompareSeries(s.View(0, s.ssize(), 6), {0, 6});
    CompareSeries(s.View(0, s.ssize(), 7), {0, 7});
    CompareSeries(s.View(0, s.ssize(), 8), {0, 8});
    CompareSeries(s.View(0, s.ssize(), 9), {0, 9});
    CompareSeries(s.View(0, s.ssize(), 10), {0});
}

TEST(SeriesView, Iterator) {
    Series s{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    Series v = s.View(1, s.size(), 2);
    Iterator i = v.begin();
    EXPECT_EQ(*i, 1);
    i++;
    EXPECT_EQ(*i, 3);
    i++;
    EXPECT_EQ(*i, 5);
    i++;
    EXPECT_EQ(*i, 7);
    i++;
    EXPECT_EQ(*i, 9);
    i++;
    EXPECT_EQ(i, v.end());

    Series v3 = s.View(0, s.size(), 3);

    Iterator i3 = v3.begin();
    EXPECT_EQ(*i3, 0);
    i3++;
    EXPECT_EQ(*i3, 3);
    i3++;
    EXPECT_EQ(*i3, 6);
    i3++;
    EXPECT_EQ(*i3, 9);
    i3++;
    EXPECT_EQ(i, v.end());
}