#pragma once
#include <unordered_map>
#include <iostream>

template <typename T, typename ID_T>
class ObjectRegistry
{
public:
    template <typename... Args>
    T *registerObject(ID_T id, Args &&...args)
    {
        auto it = objects_.find(id);
        if (it != objects_.end())
        {
            return it->second;
        }
        else
        {
            objects_[id] = new T(std::forward<Args>(args)...);
            return getObject(id);
        }
    }

    T *getObject(ID_T id)
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

    ObjectRegistry() {}
    ~ObjectRegistry()
    {
        for (auto &i : objects_)
        {
            std::cout << "object id: " << i.first << " ";
            i.second->display();
            delete i.second;
        }
    }

    std::unordered_map<ID_T, T *> objects_;
};

template <typename T>
class Singleton
{
public:
    static T &getInstance()
    {
        static T instance;
        return instance;
    }

private:
    Singleton() = default;
    ~Singleton() = default;

    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
};