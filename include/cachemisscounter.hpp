#pragma once

#include <cstdint>
#include <string>

class CacheMissCounter
{
public:
    CacheMissCounter();
    ~CacheMissCounter();

    void start();
    void stop();

    uint64_t get_cache_misses() const;

private:
    int fd_;
    uint64_t cache_misses_;
};

class AvgCacheMissCounter
{
public:
    AvgCacheMissCounter(const std::string name);
    ~AvgCacheMissCounter();

    void start();
    void stop();

    double get_avg_cache_misses() const;

private:
    std::string name_;
    CacheMissCounter cache_miss_counter_;
    uint64_t total_cache_misses_;
    uint64_t num_samples_;
};

class ScopeCacheMissCounter
{
public:
    ScopeCacheMissCounter(const std::string &name);
    ~ScopeCacheMissCounter();

private:
    std::string m_name;
    CacheMissCounter m_counter;
};

class ScopeAvgCacheMissCounter
{
public:
    ScopeAvgCacheMissCounter(AvgCacheMissCounter &avgCounter);
    ~ScopeAvgCacheMissCounter();

private:
    AvgCacheMissCounter &m_avgCounter;
};
