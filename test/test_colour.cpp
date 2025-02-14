#include "caslib/colour.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(Colour, Interpolate) {
    auto c = Colour::Interpolate({.red = 0, .green = 50, .blue = 100},
                                 {.red = 60, .green = 250, .blue = 80}, 0.25f);
    EXPECT_EQ(c.red, 15);
    EXPECT_EQ(c.green, 100);
    EXPECT_EQ(c.blue, 95);
}

TEST(Colour, Map) {
    Colour r{255, 0, 0};
    Colour g{0, 255, 0};
    Colour b{0, 0, 255};

    ContinuousColourMap cmap({r, g, b});

    EXPECT_EQ(cmap.Get(-1), r);
    EXPECT_EQ(cmap.Get(0), r);
    EXPECT_EQ(cmap.Get(0.5), g);
    EXPECT_EQ(cmap.Get(1.0), b);
    EXPECT_EQ(cmap.Get(1.5), b);

    EXPECT_EQ(cmap.Get(0.125), Colour::Interpolate(r, g, 0.25));
}
