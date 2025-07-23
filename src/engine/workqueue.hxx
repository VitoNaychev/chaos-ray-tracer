#ifndef WORKQUEUE_HXX
#define WORKQUEUE_HXX

#include <thread>
#include <vector>
#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>

class WorkQueue {
public:
    WorkQueue(int workersCount);
    
    void push(std::function<void()> task);
    void wait();

private:
    void worker();
    
    std::vector<std::thread> workers;
    std::deque<std::function<void()>> queue;

    std::mutex queueMu;
    std::condition_variable cv;
    
    bool stop = false;
};

#endif // WORKQUEUE_HXX