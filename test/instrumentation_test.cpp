#include <gtest/gtest.h>
#include "eventcounter.hpp"

TEST(EventCounterTest, CountEvents)
{
    EventCounter<uint64_t, int> counter("test counter");
    counter.count(0);
    counter.count(0);
    counter.count(0);
    EXPECT_EQ(counter.getCount(0), 3);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
