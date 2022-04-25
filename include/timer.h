#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <ctime>

class Timer
{
public:
    Timer(const char* name)
        : m_Name(name), m_Stopped(false), m_duration(0)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    long long Stop()
    {
        m_endTimepoint = std::chrono::high_resolution_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_endTimepoint).time_since_epoch().count();
        m_duration = end - start; 
        m_Stopped = true;
    }

    long long getDuration() {
        return m_duration;
    }

    double getThroughput(long long opscount) {
        return opscount / m_duration; 
    }

    auto getStart() {
        return m_endTimepoint;
    }

    auto getEnd() {
        return m_StartTimepoint;
    }

    operator std::string() const {
        std::stringstream ss;
        auto t_s = std::chrono::system_clock::to_time_t(m_StartTimepoint);
        auto t_e = std::chrono::system_clock::to_time_t(m_endTimepoint);
        ss << "Start: " << std::put_time(std::localtime(&t_s),"%F %T") << " End: " << std::put_time(std::localtime
        (&t_e),"%F %T") << " Duration: " << m_duration;
        return ss.str();
    }
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_endTimepoint;
    bool m_Stopped;
    long long m_duration;
};
