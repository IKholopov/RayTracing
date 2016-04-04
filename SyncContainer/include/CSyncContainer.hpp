#ifndef C_SYNC_CONTAINER
#define C_SYNC_CONTAINER

#include <queue>
#include <stack>
#include <mutex>
#include <atomic>
#include <condition_variable>

template <class CONTAINER>
class CSyncContainer
{
    typedef typename CONTAINER::value_type value_type;
    public:
        CSyncContainer();

        void push(value_type item);
        bool popOrSleep(value_type& item);
        bool popNoSleep(value_type& item);
        size_t size();
        void terminate();
        void restart();

    private:
        CONTAINER container_;
        std::mutex containerLock_;
        std::condition_variable notEmptyFlag_;
        std::atomic<bool> terminated_;

        template <typename T = CONTAINER>
        typename std::enable_if<
                      std::is_same<std::queue<typename CONTAINER::value_type>, T>::value ||
                      std::is_same<std::stack<typename CONTAINER::value_type>, T>::value>::type
        pushToContainer(T& container, value_type item);

        template <typename T = CONTAINER>
        typename std::enable_if<
                      !(std::is_same<std::queue<typename CONTAINER::value_type>, T>::value ||
                      std::is_same<std::stack<typename CONTAINER::value_type>, T>::value)>::type
        popFromContainer(T& container, value_type& item);

        template <typename T = CONTAINER>
        typename std::enable_if<
                      !(std::is_same<std::queue<typename CONTAINER::value_type>, T>::value ||
                      std::is_same<std::stack<typename CONTAINER::value_type>, T>::value)>::type
        pushToContainer(T& container, value_type item);

        template <typename T = CONTAINER>
        typename std::enable_if<
                      std::is_same<std::queue<typename CONTAINER::value_type>, T>::value>::type
        popFromContainer(T& container, value_type& item);

        template <typename T = CONTAINER>
        typename std::enable_if<
                      std::is_same<std::stack<typename CONTAINER::value_type>, T>::value>::type
        popFromContainer(T& container, value_type& item);

};



template <class CONTAINER>
CSyncContainer<CONTAINER>::CSyncContainer()
{
    terminated_.store(false);
}

template <class CONTAINER>
void CSyncContainer<CONTAINER>::push(value_type item)
{
    std::unique_lock<std::mutex> lock(containerLock_);
    this->pushToContainer<CONTAINER>(container_, item);
    lock.unlock();
    notEmptyFlag_.notify_all();
}

template <class CONTAINER>
bool CSyncContainer<CONTAINER>::popOrSleep(value_type& item)
{
    std::unique_lock<std::mutex> lock(containerLock_);
    while(container_.empty() && !terminated_)
        notEmptyFlag_.wait(lock);
    if(container_.empty())
        return false;
    this->popFromContainer(container_, item);
    return true;
}

template <class CONTAINER>
bool CSyncContainer<CONTAINER>::popNoSleep(value_type& item)
{
    std::unique_lock<std::mutex> lock(containerLock_);
    if(container_.empty())
        return false;
    this->popFromContainer(container_, item);
    return true;
}
template <class CONTAINER>
size_t CSyncContainer<CONTAINER>::size()
{
    std::unique_lock<std::mutex> lock(containerLock_);
    return container_.size();
}
template <class CONTAINER>
void CSyncContainer<CONTAINER>::terminate()
{
    std::unique_lock<std::mutex> lock(containerLock_);
    terminated_.store(true);
    lock.unlock();
    notEmptyFlag_.notify_all();
}
template <class CONTAINER>
void CSyncContainer<CONTAINER>::restart()
{
    std::unique_lock<std::mutex> lock(containerLock_);
    terminated_.store(false);
}
template <class CONTAINER>
template <typename T>
typename std::enable_if<
              std::is_same<std::queue<typename CONTAINER::value_type>, T>::value ||
              std::is_same<std::stack<typename CONTAINER::value_type>, T>::value>::type
CSyncContainer<CONTAINER>::pushToContainer(T& container, value_type item)
{
    container.push(item);
}

template <class CONTAINER>
template <typename T>
typename std::enable_if<
              !(std::is_same<std::queue<typename CONTAINER::value_type>, T>::value ||
              std::is_same<std::stack<typename CONTAINER::value_type>, T>::value)>::type
CSyncContainer<CONTAINER>::popFromContainer(T& container, value_type& item)
{
    item = container.back();
    container.pop_back();
}

template <class CONTAINER>
template <class T>
typename std::enable_if<
              !(std::is_same<std::queue<typename CONTAINER::value_type>, T>::value ||
              std::is_same<std::stack<typename CONTAINER::value_type>, T>::value)>::type
CSyncContainer<CONTAINER>::pushToContainer(T& container,
                                                value_type item)
{
    container.push_back(item);
}

template <class CONTAINER>
template <typename T>
typename std::enable_if<
              std::is_same<std::queue<typename CONTAINER::value_type>, T>::value>::type
CSyncContainer<CONTAINER>::popFromContainer(T& container, value_type& item)
{
    item = container.front();
    container.pop();
}

template <class CONTAINER>
template <typename T>
typename std::enable_if<
              std::is_same<std::stack<typename CONTAINER::value_type>, T>::value>::type
CSyncContainer<CONTAINER>::popFromContainer(T& container, value_type& item)
{
    item = container.top();
    container.pop();
}

#endif
