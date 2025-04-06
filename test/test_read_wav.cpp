#include "caslib/audio.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(ReadWav, Properties) {
    AudioFile f = LoadWav<float>("data/M1F1-Alaw-AFsp.wav");

    // Expected values from https://filehelper.com/
    EXPECT_EQ(f.sample_rate, 8000);
    EXPECT_EQ(f.data.cols(), 2);
    EXPECT_EQ(f.data.rows(), 23493);

    // Ensure each channel is stored contiguously
    EXPECT_EQ(f.data.col(0).innerStride(), 1);
    EXPECT_EQ(f.data.col(1).innerStride(), 1);
}