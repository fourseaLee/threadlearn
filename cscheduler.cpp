#include "cscheduler.h"
#include <assert.h>
#include <binders.h>
#include <functional>
#include "reverselock.h"



CScheduler::CScheduler():nthreads_servicing_queue_(0),stop_requested_(false),stop_when_empty_(false)
{

}

CScheduler::~CScheduler()
{
    assert(nthreads_servicing_queue_ == 0);
}

void CScheduler::schedule(CScheduler::Function f, std::chrono::system_clock::time_point t)
{
    {
        std::unique_lock<std::mutex> lock(new_task_mutex_);
        task_queue_.insert(std::make_pair(t, f));
    }
    new_task_scheduled_.notify_one();
}

void CScheduler::scheduleFromNow(CScheduler::Function f, uint64_t delat_millon_seconds)
{
    schedule(f,std::chrono::system_clock::now() + std::chrono::milliseconds(delat_millon_seconds));
}

static void Repeat(CScheduler* s, CScheduler::Function f, int64_t deltaMilliSeconds)
{
    f();
    s->scheduleFromNow(std::bind(&Repeat, s, f, deltaMilliSeconds), deltaMilliSeconds);
}

void CScheduler::scheduleEvery(CScheduler::Function f, uint64_t delat_millon_seconds)
{
    scheduleFromNow(std::bind(&Repeat, this, f, delat_millon_seconds), delat_millon_seconds);
}

void CScheduler::serviceQueue()
{

    std::unique_lock<std::mutex> lock (new_task_mutex_);
    ++nthreads_servicing_queue_;

    // newTaskMutex is locked throughout this loop EXCEPT
    // when the thread is waiting or when the user's function
    // is called.
    while (!shouldStop()) {
        try {
            if (!shouldStop() && task_queue_.empty()) {
                reverse_lock<std::unique_lock<std::mutex> > rlock(lock);
                // Use this chance to get a tiny bit more entropy
                //   RandAddSeedSleep();
                //sleep(1);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            while (!shouldStop() && task_queue_.empty()) {
                // Wait until there is something to do.
                new_task_scheduled_.wait(lock);
            }

            // Wait until either there is a new task, or until
            // the time of the first item on the queue:
            // Some boost versions have a conflicting overload of wait_until that returns void.
            // Explicitly use a template here to avoid hitting that overload.
            while (!shouldStop() && !task_queue_.empty()) {
                std::chrono::system_clock::time_point timeToWaitFor = task_queue_.begin()->first;
                if (new_task_scheduled_.wait_until<>(lock, timeToWaitFor) == std::cv_status::timeout)
                    break; // Exit loop after timeout, it means we reached the time of the event
            }
            // If there are multiple threads, the queue can empty while we're waiting (another
            // thread may service the task we were waiting on).
            if (shouldStop() || task_queue_.empty())
                continue;

            Function f = task_queue_.begin()->second;
            task_queue_.erase(task_queue_.begin());


            {
                // Unlock before calling f, so it can reschedule itself or another task
                // without deadlocking:
                reverse_lock<std::unique_lock<std::mutex> > rlock(lock);
                f();
            }
        } catch (...) {
            --nthreads_servicing_queue_;
            throw;
        }
    }
    --nthreads_servicing_queue_;
    new_task_scheduled_.notify_one();
}

void CScheduler::stop(bool drain)
{
    {
        std::unique_lock<std::mutex> lock(new_task_mutex_);
        if (drain)
            stop_when_empty_ = true;
        else
            stop_requested_ = true;
    }
    new_task_scheduled_.notify_all();
}

size_t CScheduler::getQueueInfo(std::chrono::system_clock::time_point &first, std::chrono::system_clock::time_point &last) const
{
    std::unique_lock<std::mutex> lock(new_task_mutex_);
    size_t result = task_queue_.size();
    if (!task_queue_.empty()) {
        first = task_queue_.begin()->first;
        last = task_queue_.rbegin()->first;
    }
    return result;
}

bool CScheduler::areThreadServicingQueue() const
{
    std::unique_lock<std::mutex> lock(new_task_mutex_);
    return nthreads_servicing_queue_;

}
