#include "functiontimer.hpp"
AvgFunctionTimer::AvgFunctionTimer(const std::string &name)
    : m_name(name), m_count(0), m_totalTime(0) {}

AvgFunctionTimer::~AvgFunctionTimer()
{
    std::cout << "Function name: " << m_name << " total_time: " << m_totalTime << " avg time: " << m_totalTime / static_cast<double>(m_count) << " count: " << m_count << std::endl;
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
    std::cout << " CodeBlock: " << name << " Total Time: " << totalDuration.count() << " Count: " << numExecutions << " Avg Time: " << getAvgExecutionTime() << std::endl;
}

TSCAvgFunctionTimer::TSCAvgFunctionTimer(const std::string &name)
    : m_name(name), m_count(0), m_totalTime(0) {}

TSCAvgFunctionTimer::~TSCAvgFunctionTimer() {}

void TSCAvgFunctionTimer::recordDuration(double duration)
{
    ++m_count;
    m_totalTime += duration;
    std::cout << "Function '" << m_name << "' took " << duration << " cycles on average over " << m_count << " calls.\n";
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
    return totalDuration / numExecutions;
}

TSCCodeTimer::~TSCCodeTimer()
{
    std::cout << "Code block '" << name << "' took " << totalDuration << " cycles in total over " << numExecutions << " executions, with an average of " << getAvgExecutionTime() << " cycles.\n";
}
