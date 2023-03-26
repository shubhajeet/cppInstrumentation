#include "functiontimer.hpp"
AvgFunctionTimer::AvgFunctionTimer(const std::string &name)
    : m_name(name), m_count(0), m_totalTime(0) {}

AvgFunctionTimer::~AvgFunctionTimer()
{
    std::cout << m_name << " took an average of " << m_totalTime / m_count << " microseconds over " << m_count << " calls." << std::endl;
}

void AvgFunctionTimer::recordDuration(std::chrono::microseconds duration)
{
    m_totalTime += duration.count();
    m_count++;
}

FunctionTimer::FunctionTimer(const std::string &name, AvgFunctionTimer &avgTimer)
    : m_name(name), m_avgTimer(avgTimer), m_startTime(std::chrono::high_resolution_clock::now()) {}

FunctionTimer::~FunctionTimer()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
    auto duration = end - start;
    m_avgTimer.recordDuration(std::chrono::microseconds(duration));
}

CodeTimer::CodeTimer() {}

void CodeTimer::start()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void CodeTimer::end()
{
    endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    totalDuration += duration;
    numExecutions++;
}

double CodeTimer::getAvgExecutionTime()
{
    if (numExecutions == 0)
    {
        return 0.0;
    }
    else
    {
        return totalDuration.count() / static_cast<double>(numExecutions);
    }
}
