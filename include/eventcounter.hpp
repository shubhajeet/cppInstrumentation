#pragma once
#include <iostream>
#include <unordered_map>
#include "objectregistry.hpp"

template <typename T, typename EVENT_T>
class EventCounter
{
public:
    EventCounter(const std::string name) : name_(name)
    {
    }
    void count(const EVENT_T event)
    {
        auto it = counters.find(event);
        if (it != counters.end())
        {
            it->second++;
        }
        else
        {
            counters[event] = 1;
        }
    }

    int getCount(const int event) const
    {
        auto it = counters.find(event);
        if (it != counters.end())
        {
            return it->second;
        }
        else
        {
            return 0;
        }
    }
    void display() const
    {
        for (auto event : counters)
        {
            std::cout << "counter: " << name_ << " event: " << event << " occured: " << counters[event] << std::endl;
        }
    }

private:
    std::string name_;
    std::unordered_map<EVENT_T, T> counters;
};

template <typename T, typename EVENT_ID>
class ScopeCounter
{
public:
    ScopeCounter(T &obj, EVENT_ID event) : m_obj(obj), m_event(event)
    {
    }

    ~ScopeCounter()
    {
        m_obj.count(m_event);
    }

private:
    T &m_obj;
    EVENT_ID &m_event;
};

template <typename T>
class ScopeCounterDisplay
{
public:
    ScopeCounterDisplay(T &obj) : m_obj(obj)
    {
    }

    ~ScopeCounterDisplay()
    {
        m_obj.display();
    }

private:
    T &m_obj;
};

using EventCounterRegistry = ObjectRegistry<EventCounter<uint64_t, std::string>, std::string>;