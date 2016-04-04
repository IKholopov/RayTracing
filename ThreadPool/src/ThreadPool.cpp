#include "ThreadPool.h"

#include <chrono>
#include <assert.h>
#include <exception>
#include <iostream>



ThreadPool::ThreadPool(unsigned int nThreads)
{
    terminated_.store(false);
    for(int i = 0; i < nThreads; ++i)
    {
        auto x = new std::thread([this]()
        {
            while(true)
            {
                std::function<void()> item;
                if(container_.popOrSleep(item))
                    item();
                else
                    break;
            }
        });
        threads_.push_back(x);
    }
}

ThreadPool::~ThreadPool()
{
    if(!terminated_.load())
        Terminate();
    for(auto thr = this->threads_.begin(); thr != threads_.end(); ++thr)
    {
        delete *thr;
    }
}

void ThreadPool::AddTask(std::function<void ()> task)
{
    this->container_.push(task);
}

void ThreadPool::Terminate()
{
    this->container_.terminate();
    this->terminated_.store(true);
    for(int i = 0; i < threads_.size(); ++i)
        threads_[i]->join();
    assert(container_.size() == 0);
}
