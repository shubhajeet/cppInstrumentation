#include "functiontimer.hpp"
AvgFunctionTimer::AvgFunctionTimer(const std::string &name)
    : m_name(name), m_count(0), m_totalTime(0) {}

AvgFunctionTimer::~AvgFunctionTimer()
{
    std::cout << "function_name: " << m_name << " total_time: " << m_totalTime << " avg_time: " << m_totalTime / static_cast<double>(m_count) << " count: " << m_count << std::endl;
}

void AvgFunctionTimer::recordDuration(std::chrono::nanoseconds duration)
{
    m_totalTime += duration.count();
    m_count++;
}

FunctionTimer::FunctionTimer(const std::string &name, AvgFunctionTimer &avgTimer)
    : m_name(name), m_avgTimer(avgTimer), m_startTime(std::chrono::high_resolution_clock::now()) {}

FunctionTimer::~FunctionTimer()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_startTime).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTime).time_since_epoch().count();
    auto duration = end - start;
    m_avgTimer.recordDuration(std::chrono::nanoseconds(duration));
}

CodeTimer::CodeTimer(std::string name) : name(name) {}

void CodeTimer::start()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void CodeTimer::end()
{
    endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
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

CodeTimer::~CodeTimer()
{
    std::cout << " CodeBlock: " << name << " total_time: " << totalDuration.count() << " count: " << numExecutions << " avg_time: " << getAvgExecutionTime() << std::endl;
}

TSCAvgFunctionTimer::TSCAvgFunctionTimer(const std::string &name)
    : m_name(name), m_count(0), m_totalTime(0) {}

TSCAvgFunctionTimer::~TSCAvgFunctionTimer()
{
    std::cout << "function_name: " << m_name << " total_cycles: " << m_totalTime << " avg_cycle: " << m_totalTime / static_cast<double>(m_count) << " count: " << m_count << std::endl;
}

void TSCAvgFunctionTimer::recordDuration(double duration)
{
    ++m_count;
    m_totalTime += duration;
    // std::cout << "function_name: " << m_name << " total_cycles: " << duration << " avg_time: " << duration / static_cast<double>(m_count) << " count: " << m_count << std::endl;
}

TSCFunctionTimer::TSCFunctionTimer(const std::string &name, TSCAvgFunctionTimer &avgTimer)
    : m_name(name), m_avgTimer(avgTimer), m_startTime(__rdtsc()) {}

TSCFunctionTimer::~TSCFunctionTimer()
{
    unsigned long long endTime = __rdtsc();
    double duration = static_cast<double>(endTime - m_startTime);
    m_avgTimer.recordDuration(duration);
}

TSCCodeTimer::TSCCodeTimer(std::string name)
    : name(name), startTime(0), endTime(0), totalDuration(0), numExecutions(0) {}

void TSCCodeTimer::start()
{
    startTime = __rdtsc();
}

void TSCCodeTimer::end()
{
    endTime = __rdtsc();
    double duration = static_cast<double>(endTime - startTime);
    totalDuration += duration;
    ++numExecutions;
}

double TSCCodeTimer::getAvgExecutionTime()
{
    if (numExecutions == 0)
    {
        return 0.0;
    }
    else
    {
        return totalDuration / static_cast<double>(numExecutions);
    }
}

TSCCodeTimer::~TSCCodeTimer()
{
    std::cout << "CodeBlock: " << name << " total_cycles: " << totalDuration << " count: " << numExecutions << " avg_cycle: " << getAvgExecutionTime() << std::endl;
}
