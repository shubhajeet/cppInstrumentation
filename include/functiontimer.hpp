#include <iostream>
#include <string>
#include <chrono>
class AvgFunctionTimer
{
public:
    AvgFunctionTimer(const std::string &name);
    ~AvgFunctionTimer();
    void recordDuration(std::chrono::microseconds duration);

private:
    std::string m_name;
    int m_count;
    long long m_totalTime;
};

class FunctionTimer
{
public:
    FunctionTimer(const std::string &name, AvgFunctionTimer &avgTimer);
    ~FunctionTimer();

private:
    std::string m_name;
    AvgFunctionTimer &m_avgTimer;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};

class CodeTimer
{
public:
    CodeTimer();
    void start();
    void end();
    double getAvgExecutionTime();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    std::chrono::microseconds totalDuration{0};
    int numExecutions{0};
};
