//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <map>

struct ProfileResult
{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct CounterRecord
{
    std::string Name;
    long int count;
};

struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
private:
    InstrumentationSession *m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;
    int m_counterCount;
    std::string output_format = "log";
    std::string filename = "profile.log";
    std::vector<ProfileResult> profiledata;
    std::map<std::string, CounterRecord> counterdata;
    Instrumentor();
    Instrumentor(Instrumentor &) = delete;

public:
    void BeginSession(const std::string &name, const std::string output_file = "profile.log", const std::string format = "log");
    void EndSession();
    void recordProfile(const ProfileResult &result);
    void recordCounter(const CounterRecord &cntr);
    bool counterExists(const std::string &name);
    void WriteCounter(const CounterRecord &cntr);
    void WriteProfile(const ProfileResult &result);
    CounterRecord &getCounter(const std::string name);
    void WriteHeader();
    void WriteFooter();
    static Instrumentor &Get();
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char *name);
    ~InstrumentationTimer();
    void Stop();
    long long duration();

private:
    const char *m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint, m_StopTimePointer;
    bool m_Stopped;
};

class InstrumentationCounter
{
private:
    std::string name;

public:
    InstrumentationCounter(const std::string &n);
    void reset(const std::string &n);
    InstrumentationCounter operator++();
    InstrumentationCounter operator++(int);
};