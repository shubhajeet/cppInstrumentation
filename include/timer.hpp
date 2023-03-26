#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <ctime>

class Timer
{
public:
    Timer(const char *name);
    ~Timer();
    long long Stop();
    long long getDuration();
    double getThroughput(long long opscount);
    auto getStart();
    auto getEnd();
    operator std::string() const;

private:
    const char *m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_endTimepoint;
    bool m_Stopped;
    long long m_duration;
};
