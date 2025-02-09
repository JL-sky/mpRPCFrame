#pragma once
#include<queue>
#include<mutex>
#include<condition_variable>

template <typename T>
class LockQueue
{
public:
    void push(const T& data)
    {
        std::lock_guard<std::mutex> lock(_qmutex);
        _queue.push(data);
        _qcondvariable.notify_one();
    }
    T pop()
    {
        std::unique_lock<std::mutex> lock(_qmutex);
        while(_queue.empty())
        {
            _qcondvariable.wait(lock);
        }
        T data=_queue.front();
        _queue.pop();
        return data;
    }
private:
    std::queue<T> _queue;
    std::mutex _qmutex;
    std::condition_variable _qcondvariable;
};