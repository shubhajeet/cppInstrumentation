#include "timer.hpp"
Timer::Timer(const char *name)
    : m_Name(name), m_Stopped(false), m_duration(0)
{
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    if (!m_Stopped)
        Stop();
}

long long Timer::Stop()
{
    m_endTimepoint = std::chrono::high_resolution_clock::now();
    long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_endTimepoint).time_since_epoch().count();
    m_duration = end - start;
    m_Stopped = true;
}

long long Timer::getDuration()
{
    return m_duration;
}

double Timer::getThroughput(long long opscount)
{
    return opscount / m_duration;
}

auto Timer::getStart()
{
    return m_endTimepoint;
}

auto Timer::getEnd()
{
    return m_StartTimepoint;
}

operator Timer::std::string() const
{
    std::stringstream ss;
    auto t_s = std::chrono::system_clock::to_time_t(m_StartTimepoint);
    auto t_e = std::chrono::system_clock::to_time_t(m_endTimepoint);
    ss << "Start: " << std::put_time(std::localtime(&t_s), "%F %T") << " End: " << std::put_time(std::localtime(&t_e), "%F %T") << " Duration: " << m_duration;
    return ss.str();
}
