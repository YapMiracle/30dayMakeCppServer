//
// Created by mirac on 2022/11/25.
//

#include "ThreadPool.h"

/**
 * default size =10.
 * @param size
 */
ThreadPool::ThreadPool(int size) : stop(false){
    for(int i = 0; i < size; ++i){
        threads.emplace_back(std::thread([this](){
            while(true){
                std::function<void()> task;// define a task.
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    cv.wait(lock, [this](){
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) return;
                    task = tasks.front();// get a task.
                    tasks.pop();
                }
                task();// conduct a task.
            }
        }));
    }
}

/**
 * notify all thread, and handle all rest task.
 */
ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all();
    for(std::thread &th : threads){
        if(th.joinable())
            th.join();// wait the thread finished.
    }
}

/**
 * Once add a thread, notify one thread.
 * @param func
 */
void ThreadPool::add(std::function<void()> func){
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        // throw an error
        if(stop)
            throw std::runtime_error("ThreadPoll already stop, can't add task any more");
        tasks.emplace(func);
    }
    cv.notify_one();
}
