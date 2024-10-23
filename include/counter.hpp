#pragma once
// The code by Sujit Maharjan
// Use to instrument the code.
// Implements SimpleCounter that has a name and counter
// count() -> increases counter
// display() -> outputs the count value
// getCount() -> gets the counter value
// Counter ->
// ScopeCounter ->
// ScopeDisplayCounter ->
// CounterRegistry -> Registry of Counter
#include <iostream>
#include <unordered_map>
#include "objectregistry.hpp"
#include "scope.hpp"

template <typename T>
class SimpleCounter
{
public:
    SimpleCounter(const std::string name) : name_(name)
    {
    }
    void start()
    {
        count();
    }
    void stop()
    {
        count();
    }
    inline void count()
    {
        counter++;
    }

    int getCount() const
    {
        return counter;
    }

    void display() const
    {
        std::cout << "counter: " << name_ << " occured: " << counter << std::endl;
    }

private:
    std::string name_;
    T counter = 0;
};

using Counter = SimpleCounter<uint64_t>;
using ScopeCounter = Scope<Counter>;
using ScopeDisplayCounter = ScopeDisplay<Counter>;
using CounterRegistry = ObjectRegistry<Counter, std::string>;
