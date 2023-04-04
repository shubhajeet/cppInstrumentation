template <int counter = 5>
class EventCounter
{
public:
    EventCounter(const std::string name) : name(name)
    {
        for (auto i = 0; i < counter; i++)
        {
            count[i] = 0;
        }
    }
    void countEvent(const int event)
    {
        if (event < counter)
        {
            count[event]++;
        }
    }
    int getCount(const int event) const
    {
        if (event < counter)
        {
            return count[event];
        }
    }
    ~EventCounter()
    {
        for (auto i = 0; i < counter; i++)
        {
            std::cout << "Event " << name << ": " << i << " occured: " << count[i] << std::endl;
        }
    }

private:
    std::string name;
    int count[counter];
};
