#pragma once
#include <unordered_map>

/**
template <typename T, typename ID_T>
class ObjectRegistry
{
public:
    static ObjectRegistry &getInstance()
    {
        static ObjectRegistry instance;
        return instance;
    }

    template <typename... Args>
    T &registerObject(ID_T id, Args &&...args)
    {
        objects_.emplace(std::make_pair(id, T(std::forward<Args>(args)...)));
        return getObject(id);
    }

    T &getObject(ID_T id)
    {
        auto it = objects_.find(id);
        if (it != objects_.end())
        {
            return it->second;
        }
        else
        {
            throw std::out_of_range("ObjectRegistry: ID not found.");
        }
    }

    ObjectRegistry(const ObjectRegistry &) = delete;
    ObjectRegistry &operator=(const ObjectRegistry &) = delete;

private:
    ObjectRegistry() {}
    ~ObjectRegistry() {}

    std::unordered_map<ID_T, T> objects_;
};
**/
template <typename T, int counter>
class ObjectRegistry
{
public:
    /**
    template <typename... Args>
    T &registerObject(int id, Args &&...args)
    {
        objects_ = T(std::forward<Args>(args)...);
        return getObject(id);
    };
    **/

    T *getObject(int id)
    {
        return objects_[id];
    };

    ObjectRegistry(const ObjectRegistry &) = delete;
    ObjectRegistry &operator=(const ObjectRegistry &) = delete;

    ~ObjectRegistry()
    {
        for (auto i = 0; i < counter; i++)
        {
            std::cout << "object name: " << name_ << " id: " << i << " ";
            delete objects_[i];
        }
    };
    template <typename... Args>
    ObjectRegistry(const std::string &name, Args &&...args) : name_(name)
    {
        for (auto i = 0; i < counter; i++)
        {
            objects_[i] = new T(std::forward<Args>(args)...);
        }
    }
    std::string name_;
    T *objects_[counter];
};
