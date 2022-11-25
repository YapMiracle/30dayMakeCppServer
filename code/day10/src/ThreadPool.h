//
// Created by mirac on 2022/11/25.
//

#ifndef UNTITLED_THREADPOOL_H
#define UNTITLED_THREADPOOL_H
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
private:
    /* All thread */
    std::vector<std::thread> threads;
    /* All task*/
    std::queue<std::function<void()>> tasks;
    /* synchronized */
    std::mutex tasks_mtx;
    std::condition_variable cv;

    bool stop;
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    void add(std::function<void()>);

};


#endif //UNTITLED_THREADPOOL_H
