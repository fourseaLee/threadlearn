#ifndef WORKQUEUE_H
#define WORKQUEUE_H
#include "workitem.h"
#include <mutex>
#include <condition_variable>
#include<deque>

template<typename WorkItem>
class WorkQueue
{
private:
    /** Mutex protects entire object */
    std::mutex cs_;
    std::condition_variable cond_;
    std::deque<std::unique_ptr<WorkItem>> queue_;
    bool running_;
    size_t maxDepth_;

public:
    explicit WorkQueue(size_t maxDepth) : running_(true),
        maxDepth_(maxDepth)
    {
    }

    ~WorkQueue()
    {
    }
    /** Enqueue a work item */
    bool Enqueue(WorkItem* item)
    {
        std::unique_lock<std::mutex> lock(cs_);
        if (queue_.size() >= maxDepth_) {
            return false;
        }
        queue_.emplace_back(std::unique_ptr<WorkItem>(item));
        cond_.notify_one();
        return true;
    }

    /** Thread function */
    void Run()
    {
        while (true) {
            std::unique_ptr<WorkItem> i;
            {
                std::unique_lock<std::mutex> lock(cs_);
                while (running_ && queue_.empty())
                    cond_.wait(lock);
                if (!running_)
                    break;
                i = std::move(queue_.front());
                queue_.pop_front();
            }
            (*i)();
        }
    }
    /** Interrupt and exit loops */
    void Interrupt()
    {
        std::unique_lock<std::mutex> lock(cs_);
        running_ = false;
        cond_.notify_all();
    }
};

#endif // WORKQUEUE_H
