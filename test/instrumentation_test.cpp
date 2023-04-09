#include "gtest/gtest.h"
#include "eventcounter.hpp"

TEST(EventCounterTest, CountEvents)
{
    EventCounter<1> counter("test counter");
    counter.countEvent(0);
    counter.countEvent(0);
    counter.countEvent(0);
    EXPECT_EQ(counter.getCount(0), 3);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
