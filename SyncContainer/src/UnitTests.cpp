#include "CSyncContainer.hpp"
#include <thread>
#include <functional>
#include <deque>
#include <iostream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE C_SYNC_QUEUE_TEST
#include <boost/test/unit_test.hpp>

template <class CONTAINER>
void Produce(unsigned int itemsCount, CSyncContainer<CONTAINER>& queue)
{
    for(int i = 0; i < itemsCount; ++i)
    {
        queue.push(1);
    }
}
template <typename T, class CONTAINER>
void ConsumeOrSleep(unsigned int itemsCount, CSyncContainer<CONTAINER>& queue, std::vector<T>* consumed)
{
    for(int i = 0; i < itemsCount; ++i)
    {
        T item;
        if(queue.popOrSleep(item))
            consumed->push_back(item);
        else
            break;
    }
}
template <typename T, class CONTAINER>
void ConsumeNoSleep(unsigned int itemsCount, CSyncContainer<CONTAINER>& queue, std::vector<T>* consumed)
{
    for(int i = 0; i < itemsCount; ++i)
    {
        T item;
        if(queue.popNoSleep(item))
            consumed->push_back(item);
        else
            break;
    }
}
template <typename T, class CONTAINER>
std::vector<std::thread*> GenerateProducerPool(unsigned int n, unsigned int itemsPerProducer, CSyncContainer<CONTAINER>& queue)
{
    std::vector<std::thread*> threads;
    for(int i = 0; i < n; ++i)
        threads.push_back(new std::thread(Produce<CONTAINER>, itemsPerProducer, std::ref(queue)));
    return threads;
}
template <typename T, class CONTAINER>
std::vector<std::thread*> GenerateConsumerNonSleepingPool(unsigned int n, unsigned int itemsPerConsumer, std::vector<std::vector<T>*>& items,
                                               CSyncContainer<CONTAINER>& queue)
{
    std::vector<std::thread*> threads;
    for(int i = 0; i < n; ++i)
        threads.push_back(new std::thread(ConsumeNoSleep<T, CONTAINER>, itemsPerConsumer, std::ref(queue), items[i]));
    return threads;
}
template <typename T, class CONTAINER>
std::vector<std::thread*> GenerateConsumerPool(unsigned int n, unsigned int itemsPerConsumer, std::vector<std::vector<T>*>& items,
                                               CSyncContainer<CONTAINER>& queue)
{
    std::vector<std::thread*> threads;
    for(int i = 0; i < n; ++i)
        threads.push_back(new std::thread(ConsumeOrSleep<T, CONTAINER>, itemsPerConsumer, std::ref(queue), items[i]));
    return threads;
}


template <class CONTAINER>
void TestPopOrSleep()
{
    int itemsPerProducer = 100000;
    const int nProduceres = 8;
    const int itemsPerConsumer = 50000;
    const int nConsumers = 8;
    CSyncContainer<CONTAINER> queue;
    std::vector<std::vector<int>*> consumers;
    for(int i = 0; i < nConsumers; ++i)
        consumers.push_back(new std::vector<int>());
    auto prThr1 = GenerateProducerPool<int, CONTAINER>(nProduceres, itemsPerProducer, queue);
    auto consThr1 = GenerateConsumerPool<int,CONTAINER>(nConsumers, itemsPerConsumer, consumers, queue);
    for(auto th: prThr1)
    {
        th->join();
        delete th;
    }
    for(auto th: consThr1)
    {
        th->join();
        delete th;
    }
    std::cout << "In Queue:" << queue.size() << std::endl;
    std::cout << "Consumed:";
    size_t sum = 0;
    for(int i = 0; i < nConsumers; ++i)
    {
        sum += consumers[i]->size();
        std::cout << consumers[i]->size() << " ";
    }
    std::cout << std::endl;
    std::cout << "sum: " << sum << std::endl;
    BOOST_CHECK(queue.size() + sum == itemsPerProducer * nProduceres);
    auto prThr2 = GenerateConsumerPool<int, CONTAINER>(nConsumers, itemsPerConsumer, consumers, queue);

    for(auto th: prThr2)
    {
        th->join();
        delete th;
    }
    std::cout << "In Queue:\t" << queue.size() << std::endl;
    std::cout << "Consumed:  ";
    sum = 0;
    for(int i = 0; i < nConsumers; ++i)
    {
        sum += consumers[i]->size();
        std::cout << consumers[i]->size() << " ";
    }

    std::cout << std::endl;
    std::cout << "sum: " << sum << std::endl;
    BOOST_CHECK(queue.size() == 0);
    BOOST_CHECK(sum == itemsPerProducer * nProduceres);
    std::cout << "With termination:" << std::endl;
    for(auto cons: consumers)
        cons->clear();
    prThr1 = GenerateProducerPool<int, CONTAINER>(nProduceres, itemsPerProducer, queue);
    consThr1 = GenerateConsumerPool<int,CONTAINER>(nConsumers, 10*itemsPerConsumer, consumers, queue);
    for(auto th: prThr1)
    {
        th->join();
        delete th;
    }
    queue.terminate();
    for(auto th: consThr1)
    {
        th->join();
        delete th;
    }
    std::cout << "In Queue:" << queue.size() << std::endl;
    std::cout << "Consumed:";
    sum = 0;
    for(int i = 0; i < nConsumers; ++i)
    {
        sum += consumers[i]->size();
        std::cout << consumers[i]->size() << " ";
    }
    std::cout << std::endl;
    std::cout << "sum: " << sum << std::endl;
    BOOST_CHECK(queue.size() == 0);
    BOOST_CHECK(queue.size() + sum == itemsPerProducer * nProduceres);

    for(auto cons: consumers)
        cons->clear();
    std::cout << "With restart" << std::endl;
    queue.restart();
    consThr1 = GenerateConsumerPool<int,CONTAINER>(nConsumers, 10*itemsPerConsumer, consumers, queue);
    prThr1 = GenerateProducerPool<int, CONTAINER>(nProduceres, itemsPerProducer, queue);
    for(auto th: prThr1)
    {
        th->join();
        delete th;
    }
    queue.terminate();
    for(auto th: consThr1)
    {
        th->join();
        delete th;
    }
    std::cout << "In Queue:" << queue.size() << std::endl;
    std::cout << "Consumed:";
    sum = 0;
    for(int i = 0; i < nConsumers; ++i)
    {
        sum += consumers[i]->size();
        std::cout << consumers[i]->size() << " ";
    }
    std::cout << std::endl;
    std::cout << "sum: " << sum << std::endl;
    BOOST_CHECK(queue.size() == 0);
    BOOST_CHECK(queue.size() + sum == itemsPerProducer * nProduceres);
    for(auto cons: consumers)
        cons->clear();
    std::cout << "Without restart" << std::endl;
    consThr1 = GenerateConsumerPool<int,CONTAINER>(nConsumers, 10*itemsPerConsumer, consumers, queue);
    for(auto th: consThr1)
    {
        th->join();
        delete th;
    }
    prThr1 = GenerateProducerPool<int, CONTAINER>(nProduceres, itemsPerProducer, queue);
    for(auto th: prThr1)
    {
        th->join();
        delete th;
    }
    std::cout << "In Queue:" << queue.size() << std::endl;
    std::cout << "Consumed:";
    sum = 0;
    for(int i = 0; i < nConsumers; ++i)
    {
        sum += consumers[i]->size();
        std::cout << consumers[i]->size() << " ";
    }
    std::cout << std::endl;
    std::cout << "sum: " << sum << std::endl;
    BOOST_CHECK(sum == 0);
    BOOST_CHECK(queue.size() + sum == itemsPerProducer * nProduceres);
    for(auto cons: consumers)
        delete cons;
}
template <class CONTAINER>
void TestPopNoSleep()
{
    int itemsPerProducer = 100000;
    const int nProduceres = 4;
    const int itemsPerConsumer = 50000;
    const int nConsumers = 8;
    CSyncContainer<CONTAINER> queue;
    std::vector<std::vector<int>*> consumers;
    for(int i = 0; i < nConsumers; ++i)
        consumers.push_back(new std::vector<int>());
    auto prThr1 = GenerateProducerPool<int, CONTAINER>(nProduceres, itemsPerProducer, queue);
    auto consThr1 = GenerateConsumerNonSleepingPool<int, CONTAINER>(nConsumers, itemsPerConsumer, consumers, queue);
    for(auto th: prThr1)
    {
        th->join();
        delete th;
    }
    for(auto th: consThr1)
    {
        th->join();
        delete th;
    }
    std::cout << "In Queue:" << queue.size() << std::endl;
    std::cout << "Consumed:";
    size_t sum = 0;
    for(int i = 0; i < nConsumers; ++i)
    {
        sum += consumers[i]->size();
        std::cout << consumers[i]->size() << " ";
    }
    std::cout << std::endl;
    std::cout << "sum: " << sum << std::endl;
    BOOST_CHECK(queue.size() + sum == itemsPerProducer * nProduceres);
    auto prThr2 = GenerateConsumerNonSleepingPool<int, CONTAINER>(nConsumers, itemsPerConsumer, consumers, queue);

    for(auto th: prThr2)
    {
        th->join();
        delete th;
    }
    std::cout << "In Queue:\t" << queue.size() << std::endl;
    std::cout << "Consumed:  ";
    sum = 0;
    for(int i = 0; i < nConsumers; ++i)
    {
        sum += consumers[i]->size();
        std::cout << consumers[i]->size() << " ";
    }
    std::cout << std::endl;
    std::cout << "sum: " << sum << std::endl;
    BOOST_CHECK(queue.size() == 0);
    BOOST_CHECK(sum == itemsPerProducer * nProduceres);
    for(auto cons: consumers)
        delete cons;
}
BOOST_AUTO_TEST_CASE(popOrSleepList)
{
    std::cout << "popOrSleepList" << std::endl;
    TestPopOrSleep<std::list<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popNoSleepList)
{
    std::cout << "popNoSleepList" << std::endl;
    TestPopNoSleep<std::list<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popOrSleepVector)
{
    std::cout << "popOrSleepVector" << std::endl;
    TestPopOrSleep<std::vector<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popNoSleepVector)
{
    std::cout << "popNoSleepVector" << std::endl;
    TestPopNoSleep<std::vector<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popOrSleepDeque)
{
    std::cout << "popOrSleepDeque" << std::endl;
    TestPopOrSleep<std::deque<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popNoSleepDeque)
{
    std::cout << "popNoSleepDeque" << std::endl;
    TestPopNoSleep<std::deque<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popOrSleepQueue)
{
    std::cout << "popOrSleepQueue" << std::endl;
    TestPopOrSleep<std::queue<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popNoSleepQueue)
{
    std::cout << "popNoSleepQueue" << std::endl;
    TestPopNoSleep<std::queue<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popOrSleepStack)
{
    std::cout << "popOrSleepStack" << std::endl;
    TestPopOrSleep<std::stack<int>>();
    std::cout << std::endl;
}
BOOST_AUTO_TEST_CASE(popNoSleepStack)
{
    std::cout << "popNoSleepStack" << std::endl;
    TestPopNoSleep<std::stack<int>>();
    std::cout << std::endl;
}
