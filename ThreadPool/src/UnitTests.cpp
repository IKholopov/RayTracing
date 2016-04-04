#include <thread>
#include <functional>
#include <deque>
#include <iostream>

#include "ThreadPool.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE C_SYNC_QUEUE_TEST
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Thread_Pool_Test)
{
    unsigned int n = 400000;
    ThreadPool pool(4);
    bool answers[n];
    for(int i = 0; i < n; ++i)
        answers[i] = false;
    for(int i = 0; i < n; ++i)
    {
       pool.AddTask([=, &answers]()
        {
            answers[i] = true;
        });
    }
    pool.Terminate();
    for(int i = 0; i < n; ++i)
    {
        if(!answers[i])
            std::cout << i << std::endl;
        BOOST_CHECK(answers[i] == true);
    }
}
