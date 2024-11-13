#pragma once
// Code by Sujit Maharjan
// Implements SimpleObjectRegistry
// registerObject -> takes id and forwards argument to object created

#include <unordered_map>
#include <map>
#include <iostream>
/**
 *
 */

template <typename T, size_t size = 5>
class SimpleObjectRegistry
{
public:
    template <typename... Args>
    T *registerObject(size_t id, Args &&...args)
    {
        if (objects_[id] == nullptr)
        {
        }
        objects_[id] = new T(std::forward<Args>(args)...);
        return getObject(id);
    }

    T *getObject(size_t id)
    {
        if (id >= size)
        {
            throw std::out_of_range("ObjectRegistry: ID not found.");
        }
        else
        {
            return objects_[id];
        }
    }

    SimpleObjectRegistry(const SimpleObjectRegistry &) = delete;
    SimpleObjectRegistry &operator=(const SimpleObjectRegistry &) = delete;

    SimpleObjectRegistry(std::string name = "global") : name_(name) {}
    ~SimpleObjectRegistry()
    {
        for (auto i = 0; i < size; i++)
        {

            auto obj = objects_[i];
            if (obj == nullptr)
            {
                continue;
            }
            std::cout << "register: " << name_ << " objectid: " << i << " ";
            obj->display();
            delete obj;
        }
    }
    void display()
    {
        for (auto i = 0; i < size; i++)
        {

            auto obj = objects_[i];
            if (obj == nullptr)
            {
                continue;
            }
            std::cout << "register: " << name_ << " objectid: " << i << " ";
            obj->display();
        }
    }
    std::string name_;
    T *objects_[size];
};

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
    void resetObject(ID_T id)
    {
        auto it = objects_.find(id);
        if (it != objects_.end())
        {
            it->second->reset();
        }
        else
        {
            throw std::out_of_range("ObjectRegistry: ID not found.");
        }
    }

    template <typename... Args>
    T *getObjectorCreate(ID_T id, Args &&...args)
    {
        auto it = objects_.find(id);
        if (it != objects_.end())
        {
            return it->second;
        }
        else
        {
            return registerObject(id, std::forward<Args>(args)...);
        }
    }

    ObjectRegistry(const ObjectRegistry &) = delete;
    ObjectRegistry &operator=(const ObjectRegistry &) = delete;

    ObjectRegistry(std::string name = "global") : name_(name) {}
    ~ObjectRegistry()
    {
        display();
        for (auto &i : objects_)
        {
            delete i.second;
        }
    }
    void reset()
    {
        for (auto &i : objects_)
        {
            std::cout << "register: " << name_ << " objectid: " << i.first << " ";
            i.second->reset();
            // delete i.second;
        }
    }
    void display()
    {
        for (auto &i : objects_)
        {
            std::cout << "register: " << name_ << " objectid: " << i.first << " ";
            i.second->display();
        }
    }
    std::string name_;
    std::map<ID_T, T *> objects_;
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