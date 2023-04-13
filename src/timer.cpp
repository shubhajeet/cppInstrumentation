#include "timer.hpp"

#include <iostream>
#include <fstream>
#include <chrono>

// Returns the TSC clock frequency in Hz
unsigned long long getTscFrequency()
{
    std::ifstream cpuinfo("/sys/devices/system/cpu/cpu0/tsc_freq_khz");
    std::string line;
    std::getline(cpuinfo, line);
    // std::cout << "string from file: " << line << std::endl;
    unsigned long long freq = (unsigned long long)std::stoi(line) * 1000;
    // std::cout << "int from file: " << freq << std::endl;
    return freq;
}

unsigned long long tsc_frequency = getTscFrequency();

Timer::Timer(const std::string &name)
    : m_Name(name), m_Stopped(false), m_duration(0)
{
}

void Timer::start()
{
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    if (!m_Stopped)
        stop();
}

void Timer::stop()
{
    m_endTimepoint = std::chrono::high_resolution_clock::now();
    long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_endTimepoint).time_since_epoch().count();
    m_duration = end - start;
    m_Stopped = true;
}

long long Timer::getDuration() const
{
    return m_duration;
}

double Timer::getThroughput(long long opscount) const
{
    return opscount / m_duration;
}

auto Timer::getStart() const
{
    return m_endTimepoint;
}

auto Timer::getEnd() const
{
    return m_StartTimepoint;
}

Timer::operator std::string() const
{
    std::stringstream ss;
    auto t_s = std::chrono::system_clock::to_time_t(m_StartTimepoint);
    auto t_e = std::chrono::system_clock::to_time_t(m_endTimepoint);
    ss << "Timer_name: " << m_Name << " Start: " << std::put_time(std::localtime(&t_s), "%F %T") << " End: " << std::put_time(std::localtime(&t_e), "%F %T") << " Duration: " << m_duration;
    return ss.str();
}

void Timer::display() const
{
    std::cout << std::string(*this) << std::endl;
}

void TSCTimer::start()
{
    asm volatile("rdtsc"
                 : "=a"(start_cycles_low), "=d"(start_cycles_high));
    start_cycles = (static_cast<unsigned long long>(start_cycles_high) << 32) | start_cycles_low;
}

void TSCTimer::stop()
{
    asm volatile("rdtsc"
                 : "=a"(end_cycles_low), "=d"(end_cycles_high));
    end_cycles = (static_cast<unsigned long long>(end_cycles_high) << 32) | end_cycles_low;
}

long long TSCTimer::getDuration() const
{
    unsigned long long elapsed_cycles = end_cycles - start_cycles;
    // double elapsed_time_ns = elapsed_cycles / (double) CPU_FREQUENCY2800000000 * 1000000000;
    double elapsed_time_ns = elapsed_cycles / (double)tsc_frequency * 10e9;
    return elapsed_time_ns;
}
void TSCTimer::display() const
{
    auto elapsed_time_ns = getDuration();
    std::cout << "Name: " << name_ << " Elapsed time: " << elapsed_time_ns << " ns tsc_freq: " << tsc_frequency << std::endl;
}

TSCTimer::TSCTimer(std::string name) : name_(name)
{
}

TSCTimer::~TSCTimer()
{
}
