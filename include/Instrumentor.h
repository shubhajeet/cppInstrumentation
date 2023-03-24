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
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;
    int m_counterCount;
    std::string output_format = "log";
    std::string filename = "profile.log";
    std::vector<ProfileResult> profiledata;  
    std::map<std::string,CounterRecord> counterdata;

    Instrumentor()
        : m_CurrentSession(nullptr), m_ProfileCount(0), m_counterCount(0)
    {
    }
    Instrumentor(Instrumentor &) = delete;

public:
    void BeginSession(const std::string& name, const std::string output_file="profile.log", const std::string format="log")
    {
        output_format = format;
        filename = output_file;
        if(filename == "") {
            //m_OutputStream = std::ostream(nullptr);
            //m_OutputStream.rdbuf(std::cout.rdbuf()); // uses cout's buffer
            //m_OutputStream = std::ostream(std::cout.rdbuf());
            ;
        }
        else {
            m_OutputStream.open(filename);
        }
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{ name };
    }

    void EndSession()
    {
        for (auto res: profiledata) {
            WriteProfile(res);
        }
        if (output_format == "csv") {
            m_OutputStream << "cat,name,count" << std::endl;            
        }
        for (auto ctr: counterdata) {
            WriteCounter(ctr.second);
        }
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void recordProfile(const ProfileResult& result){
       profiledata.push_back(result); 
    }

    void recordCounter(const CounterRecord& cntr) {
        counterdata[cntr.Name] = cntr;
    }

    bool counterExists(const std::string& name) {
        return counterdata.count(name) > 0;
    }

    void WriteCounter(const CounterRecord& cntr) {
        if (output_format == "json") {
            if (m_counterCount++ > 0) {
                m_OutputStream << "," << std::endl;
            }
            std::string name = cntr.Name;
            std::replace(name.begin(), name.end(), '"', '\'');
            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"counter\",";
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"count\":\"" << cntr.count << "\",";
            m_OutputStream << "}";
        }
        else if (output_format == "csv") {
            m_OutputStream << "counter" << "," << cntr.Name << "," << cntr.count << std::endl;
        }
        else {
            m_OutputStream << "cat: counter name:" << cntr.Name << " count: " << cntr.count << std::endl;
        }
    }

    void WriteProfile(const ProfileResult& result)
    {
        if (output_format == "json") {
            if (m_ProfileCount++ > 0)
                m_OutputStream << "," << std::endl;

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');
        
            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start << ",";
            m_OutputStream << "\"te\":" << result.End;
            m_OutputStream << "}";
        }
        else if (output_format == "csv") {
            m_OutputStream << "function," << (result.End - result.Start) << "," << result.Name << ",X," <<"0,"<<result.ThreadID << "," << result.Start << "," << result.End << std::endl;
        }
        else {
            m_OutputStream << "cat: function dur:" << (result.End - result.Start) << " name: " << result.Name << " ph: X" <<"pid: 0 tid: "<<result.ThreadID << " ts: " << result.Start << " te: " << result.End << std::endl;
        }
        //m_OutputStream.flush();
    }

    CounterRecord& getCounter(const std::string name) {
        return counterdata[name];
    }

    void WriteHeader()
    {
        if (output_format == "json") {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        }
        else if (output_format == "csv") {
            m_OutputStream << "cat,dur,name,ph,pid,tid,ts,te" << std::endl;
        }
        else if (output_format == "log") {
        ;
        }
        m_OutputStream.flush();
    }

    void WriteFooter()
    {
        if (output_format == "json") {
            m_OutputStream << "]}";
        }
        m_OutputStream.flush();
    }

    static Instrumentor& Get()
    {
        static Instrumentor instance;
        return instance;
    }
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        m_StopTimePointer = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StopTimePointer).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        struct ProfileResult pr;
        pr.Name = m_Name;
        pr.Start = start;
        pr.End = end;
        pr.ThreadID =  threadID;
        Instrumentor::Get().recordProfile(pr);

        m_Stopped = true;
    }

    long long duration() {
        auto endTimepoint = m_StopTimePointer;
        if (!m_Stopped) {
            endTimepoint = std::chrono::high_resolution_clock::now();
        }
        long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimepoint).time_since_epoch().count();
        return end-start;
    }
    
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint, m_StopTimePointer;
    bool m_Stopped;
};

class InstrumentationCounter {
    private:
    std::string name;
    public:
    InstrumentationCounter(const std::string& n): name(n) {
        if (!Instrumentor::Get().counterExists(n)) {
            reset(name);
        }
    }
    void reset(const std::string& n) {
        Instrumentor::Get().recordCounter({name,0});
    }
    InstrumentationCounter operator ++() {
        Instrumentor::Get().getCounter(name).count++;
        return *this;
    }
    InstrumentationCounter operator ++(int) {
        Instrumentor::Get().getCounter(name).count++;
        return *this;
    } 

};