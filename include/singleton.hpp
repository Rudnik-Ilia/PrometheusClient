#pragma once

#include <atomic>
#include <functional>
#include <mutex>

#include "init.hpp"

template<class T>
class Singleton: public Delete
{
    public:
        static T *GetInstance();
        
    private:
        static std::atomic<T*> s_ptr;
        static void CleanUp();
};

template<class T>
std::atomic<T*> Singleton<T>::s_ptr;

template<class T>
T* Singleton<T>::GetInstance()
{
    static std::mutex s_mutex;

    T *tmp = s_ptr.load(std::memory_order_acquire);

    if(!tmp)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        tmp = s_ptr.load(std::memory_order_relaxed);

        if(!tmp)
        {
            tmp = new T();
            s_ptr.store(tmp, std::memory_order_release);
            atexit(CleanUp);
        }
    }
    return tmp;
}

template<class T>
void Singleton<T>::CleanUp()
{
    delete s_ptr.load();
}