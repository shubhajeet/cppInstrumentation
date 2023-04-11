template <typename T>
class Scope
{
public:
    Scope(T &obj) : m_obj(obj)
    {
        obj.start();
    }

    ~Scope()
    {
        obj.stop();
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
        obj.start();
    }

    ~ScopeDisplay()
    {
        obj.stop();
        obj.display();
    }

private:
    T &m_obj;
};