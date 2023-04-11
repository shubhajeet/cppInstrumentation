#pragma once
template <typename T>
class Scope
{
public:
    Scope(T &obj) : m_obj(obj)
    {
        m_obj.start();
    }

    ~Scope()
    {
        m_obj.stop();
    }

private:
    T &m_obj;
};

template <typename T>
class ScopeDisplay
{
public:
    ScopeDisplay(T &obj) : m_obj(obj)
    {
        m_obj.start();
    }

    ~ScopeDisplay()
    {
        m_obj.stop();
        m_obj.display();
    }

private:
    T &m_obj;
};