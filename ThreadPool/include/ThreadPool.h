#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <functional>
#include <thread>
#include <vector>

#include "CSyncContainer.hpp"

class ThreadPool
{
    public:
        ThreadPool(unsigned int nThreads);
        ~ThreadPool();
        void AddTask(std::function<void()> task);
        void Terminate();
        void WaitAll();

    private:
        std::atomic<bool> terminated_;
        CSyncContainer<std::vector<std::function<void()> > > container_;
        std::vector<std::thread*> threads_;
};

#endif
