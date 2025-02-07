//
// Created by Laky64 on 01/08/2023.
//

#include "dispatch_queue.hpp"

DispatchQueue::DispatchQueue(std::string name, size_t threadCount) :
        name{std::move(name)}, threads(threadCount) {
    for(auto & i : threads) {
        i = std::thread(&DispatchQueue::dispatchThreadHandler, this);
    }
}

DispatchQueue::~DispatchQueue() {
    std::unique_lock<std::mutex> lock(lockMutex);
    quit = true;
    lock.unlock();
    condition.notify_all();

    for(auto & thread : threads) {
        if(thread.joinable()) {
            thread.join();
        }
    }
}

void DispatchQueue::dispatch(const fp_t& op) {
    std::unique_lock<std::mutex> lock(lockMutex);
    queue.push(op);
    lock.unlock();
    condition.notify_one();
}

void DispatchQueue::dispatch(fp_t&& op) {
    std::unique_lock<std::mutex> lock(lockMutex);
    queue.push(std::move(op));
    lock.unlock();
    condition.notify_one();
}

void DispatchQueue::dispatchThreadHandler() {
    std::unique_lock<std::mutex> lock(lockMutex);
    do {
        condition.wait(lock, [this]{
            return (!queue.empty() || quit);
        });
        if(!quit && !queue.empty()) {
            auto op = std::move(queue.front());
            queue.pop();
            lock.unlock();
            op();
            lock.lock();
        }
    } while (!quit);
}
