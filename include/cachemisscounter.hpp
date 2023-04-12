#pragma once

#include <cstdint>
#include <string>
#include "objectregistry.hpp"
#include "scope.hpp"

class CacheMissCounter
{
public:
    CacheMissCounter(const std::string name);
    ~CacheMissCounter();

    void start();
    void stop();
    void display() const;
    uint64_t get_cache_misses() const;

private:
    int fd_;
    uint64_t cache_misses_;
    std::string name_;
};

class AvgCacheMissCounter
{
public:
    AvgCacheMissCounter(const std::string name);
    ~AvgCacheMissCounter();

    void start();
    void stop();
    void display() const;

    double get_avg_cache_misses() const;

private:
    std::string name_;
    CacheMissCounter cache_miss_counter_;
    uint64_t total_cache_misses_;
    uint64_t num_samples_;
};

using ScopeCacheMissCounter = Scope<CacheMissCounter>;
using ScopeDisplayCacheMissCounter = ScopeDisplay<CacheMissCounter>;
using ScopeAvgCacheMissCounter = Scope<AvgCacheMissCounter>;
using ScopeDisplayAvgCacheMissCounter = ScopeDisplay<AvgCacheMissCounter>;

using CacheMissCounterRegistry = ObjectRegistry<CacheMissCounter, std::string>;
using AvgCacheMissCounterRegistry = ObjectRegistry<AvgCacheMissCounter, std::string>;