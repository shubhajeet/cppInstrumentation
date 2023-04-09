#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <x86intrin.h>

#ifdef INSTRUMENTATION
#define INS_BLOCK() if constexp (true)
#else
#define INS_BLOCK() if constexp (false)
#endif

class AvgFunctionTimer
{
public:
    AvgFunctionTimer(const std::string &name);
    ~AvgFunctionTimer();
    void recordDuration(std::chrono::nanoseconds duration);

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
    CodeTimer(std::string name);
    void start();
    void end();
    double getAvgExecutionTime();
    ~CodeTimer();

private:
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    std::chrono::nanoseconds totalDuration{0};
    int numExecutions{0};
};

class TSCAvgFunctionTimer
{
public:
    TSCAvgFunctionTimer(const std::string &name);
    ~TSCAvgFunctionTimer();
    void recordDuration(double duration);

private:
    std::string m_name;
    int m_count;
    double m_totalTime;
};

class TSCFunctionTimer
{
public:
    TSCFunctionTimer(const std::string &name, TSCAvgFunctionTimer &avgTimer);
    ~TSCFunctionTimer();

private:
    std::string m_name;
    TSCAvgFunctionTimer &m_avgTimer;
    unsigned long long m_startTime;
};

class TSCCodeTimer
{
public:
    TSCCodeTimer(std::string name);
    void start();
    void end();
    double getAvgExecutionTime();
    ~TSCCodeTimer();

private:
    std::string name;
    unsigned long long startTime;
    unsigned long long endTime;
    double totalDuration{0};
    int numExecutions{0};
};
