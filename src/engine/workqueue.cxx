#include "workqueue.hxx"

#include <thread>
#include <vector>
#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>

WorkQueue::WorkQueue(int workersCount) {
    for (int i = 0; i < workersCount; ++i) {
        workers.push_back(std::thread([this]() { this->worker(); }));
    }
}

void WorkQueue::push(std::function<void()> task) {
    {
        const std::lock_guard<std::mutex> lock(queueMu);
        queue.push_back(task);
    }
    cv.notify_one();
}

void WorkQueue::worker() {
    while (true) {

        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMu);
            cv.wait(lock, [this]() {
                return stop || !queue.empty();
            });

            if (stop && queue.empty()) {
                return;
            }

            task = std::move(queue.front());
            queue.pop_front();
        }
        task();
    }
   
}

void WorkQueue::wait() {
    {
        std::lock_guard<std::mutex> lock(queueMu);
        stop = true;
    }
    cv.notify_all();

    for (auto& worker : workers){ 
        worker.join(); 
    }
}