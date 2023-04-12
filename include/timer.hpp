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
#include "avg.hpp"

class Timer
{
public:
    Timer(const char *name);
    ~Timer();
    void start();
    void stop();
    void display() const;
    long long getDuration() const;
    double getThroughput(long long opscount) const;
    auto getStart() const;
    auto getEnd() const;
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
    long long getDuration() const;

private:
    unsigned int start_cycles_low, start_cycles_high, end_cycles_low, end_cycles_high;
    unsigned long long start_cycles, end_cycles;
    std::string name_;
};

using AvgTimer = Avg<Timer>;
using AvgTSCTimer = Avg<TSCTimer>;
using ScopeAvgTimer = Scope<AvgTimer>;
using ScopeAvgTSCTimer = Scope<AvgTSCTimer>;
using ScopeDisplayAvgTimer = ScopeDisplay<AvgTimer>;
using ScopeDisplayAvgTSCTimer = ScopeDisplay<AvgTSCTimer>;

using ScopeTimer = Scope<Timer>;
using ScopeDisplayTimer = ScopeDisplay<Timer>;
using ScopeTSCTimer = Scope<TSCTimer>;
using ScopeDisplayTSCTimer = ScopeDisplay<TSCTimer>;

using TimerRegistry = ObjectRegistry<Timer, std::string>;
using AvgTimerRegistry = ObjectRegistry<AvgTimer, std::string>;
using TSCTimerRegistry = ObjectRegistry<TSCTimer, std::string>;
using AvgTSCTimerRegistry = ObjectRegistry<AvgTSCTimer, std::string>;