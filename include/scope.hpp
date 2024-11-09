#pragma once
// Code by Sujit Maharjan
// Scope -> call start at creation and stop at destruction
// ScopeDisplay -> call start at creation and stop and display at destruction
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

    T *operator->()
    {
        return &m_obj;
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

    T *operator->()
    {
        return &m_obj;
    }

private:
    T &m_obj;
};