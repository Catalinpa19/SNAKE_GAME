#include "app/NumberFormat.h"

#include <gtest/gtest.h>

namespace
{
struct FormatCase
{
    qint64 value;
    const char* expected;
};

class NumberFormatTest : public ::testing::TestWithParam<FormatCase>
{
};
} // namespace

TEST_P(NumberFormatTest, FormatsAndTruncatesAtUnitBoundaries)
{
    EXPECT_EQ(formatCount(GetParam().value), GetParam().expected);
}

INSTANTIATE_TEST_SUITE_P(Boundaries, NumberFormatTest,
                         ::testing::Values(FormatCase{-1, "-1"}, FormatCase{0, "0"}, FormatCase{1, "1"},
                                           FormatCase{999, "999"}, FormatCase{1000, "1k"}, FormatCase{1099, "1k"},
                                           FormatCase{1100, "1.1k"}, FormatCase{1500, "1.5k"},
                                           FormatCase{999999, "999.9k"}, FormatCase{1000000, "1m"},
                                           FormatCase{1999999, "1.9m"}, FormatCase{1000000000, "1b"},
                                           FormatCase{2147483647, "2.1b"}));
