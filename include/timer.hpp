#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "scope.hpp"
#include "objectregistry.hpp"

class Timer
{
public:
    Timer(const char *name);
    ~Timer();
    void start();
    void stop();
    void display() const;
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

class TSCTimer
{
public:
    TSCTimer(std::string name);
    ~TSCTimer();
    void start();
    void stop();
    void display() const;

private:
    unsigned int start_cycles_low, start_cycles_high, end_cycles_low, end_cycles_high;
    unsigned long long start_cycles, end_cycles;
    std::string name_;
};

using ScopeTimer = Scope<Timer>;
using ScopeDisplayTimer = ScopeDisplay<Timer>;
using ScopeTSCTimer = Scope<TSCTimer>;
using ScopeDisplayTSCTimer = ScopeDisplay<TSCTimer>;

using TimerRegistry = ObjectRegistry<Timer, std::string>;
using TSCTimerRegistry = ObjectRegistry<TSCTimer, std::string>;