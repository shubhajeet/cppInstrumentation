#include "cachemisscounter.hpp"

#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

CacheMissCounter::CacheMissCounter(const std::string name)
    : fd_(-1), cache_misses_(0), name_(name)
{
    struct perf_event_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.type = PERF_TYPE_HW_CACHE;
    attr.config = PERF_COUNT_HW_CACHE_MISSES |
                  (PERF_COUNT_HW_CACHE_L1D << 16) |
                  (PERF_COUNT_HW_CACHE_OP_READ << 24) |
                  (PERF_COUNT_HW_CACHE_RESULT_MISS << 0);
    attr.size = sizeof(attr);
    attr.disabled = 1;
    attr.exclude_kernel = 1;
    attr.exclude_hv = 1;
    fd_ = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0);
    if (fd_ == -1)
    {
        throw std::runtime_error("perf_event_open() failed");
    }
}

CacheMissCounter::~CacheMissCounter()
{
    close(fd_);
}

void CacheMissCounter::start()
{
    ioctl(fd_, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd_, PERF_EVENT_IOC_ENABLE, 0);
}

void CacheMissCounter::stop()
{
    ioctl(fd_, PERF_EVENT_IOC_DISABLE, 0);
    ssize_t ret = read(fd_, &cache_misses_, sizeof(cache_misses_));
    if (ret != sizeof(cache_misses_))
    {
        throw std::runtime_error("read() failed");
    }
}

uint64_t CacheMissCounter::get_cache_misses() const
{
    return cache_misses_;
}

AvgCacheMissCounter::AvgCacheMissCounter(const std::string name)
    : total_cache_misses_(0), num_samples_(0), name_(name), cache_miss_counter_(name)
{
}

void AvgCacheMissCounter::display() const
{
    std::cout << "AvgCacheMissCounter: " << name_ << " cache_misses: " << total_cache_misses_ << " count: " << num_samples_ << " avg_cache_misses: " << get_avg_cache_misses() << std::endl;
}

AvgCacheMissCounter::~AvgCacheMissCounter()
{
}

void AvgCacheMissCounter::start()
{
    cache_miss_counter_.start();
}

void AvgCacheMissCounter::stop()
{
    cache_miss_counter_.stop();
    total_cache_misses_ += cache_miss_counter_.get_cache_misses();
    num_samples_++;
}

double AvgCacheMissCounter::get_avg_cache_misses() const
{
    if (num_samples_ == 0)
    {
        return 0.0;
    }
    return static_cast<double>(total_cache_misses_) / static_cast<double>(num_samples_);
}
