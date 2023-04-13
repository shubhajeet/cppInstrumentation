#include <gtest/gtest.h>
#include "eventcounter.hpp"
#include "timer.hpp"

TEST(TSCTimerTest, MeasuresOneSecond)
{
    constexpr auto seconds = 5;
    constexpr double tolerance = 0.1; // Allow 10% error    TSCTimer timer("TestTimer");
    std::cout << "TSC frequency: " << getTscFrequency() << std::endl;
    TSCTimer timer("TestTimer");
    timer.start();
    sleep(seconds); // Wait for 1 second
    timer.stop();
    timer.display();
    std::cout << "TSC frequency static: " << tsc_frequency << std::endl;
    double duration_ns = timer.getDuration();
    auto expected_duration_ns = seconds * 10e9;
    double error = std::abs(duration_ns - expected_duration_ns) / expected_duration_ns;
    ASSERT_LE(error, tolerance) << "Measured duration: " << duration_ns << "ns, expected duration: " << expected_duration_ns << "ns";
}

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
