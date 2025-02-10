#include "caslib/file/wav.hpp"
#include "gtest/gtest.h"

using namespace cas;

TEST(ReadWav, Properties) {
    AudioFile f = ReadWav("data/M1F1-Alaw-Afsp.wav");

    // Expected values from https://filehelper.com/
    EXPECT_EQ(f.channels.size(), 2);
    EXPECT_EQ(f.sample_rate, 8000);
    EXPECT_EQ(f.channels[0].size(), 23493);
}