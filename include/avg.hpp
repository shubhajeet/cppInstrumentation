#pragma once
template <typename T>
class Avg
{
public:
    template <typename... Args>
    Avg(const std::string &name, Args... args) : m_name(name), m_count(0), m_total(0), m_obj(name, std::forward<Args>(args)...){};
    ~Avg(){};
    void start()
    {
        m_obj.start();
    };
    void stop()
    {
        m_obj.stop();
        recordDuration(m_obj.getDuration());
    };
    void display() const
    {
        std::cout << "AvgTimer: " << m_name << " avg: " << m_total / static_cast<float>(m_count) << " count: " << m_count << " total: " << m_total << std::endl;
    };
    void recordDuration(long long duration)
    {
        m_total += duration;
        m_count++;
    }

private:
    std::string m_name;
    int m_count;
    long long m_total;
    T m_obj;
};