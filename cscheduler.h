#ifndef CSCHEDULER_H
#define CSCHEDULER_H
#include <thread>
#include <chrono>
#include <map>
#include <condition_variable>
#include <mutex>

class CScheduler
{
public:
    typedef std::function<void(void)>  Function;
public:
    CScheduler();
    ~CScheduler();

    void schedule(Function f, std::chrono::system_clock::time_point t = std::chrono::system_clock::now());

    void scheduleFromNow(Function f, uint64_t delat_millon_seconds);

    void scheduleEvery(Function f, uint64_t delat_millon_seconds);

    void serviceQueue();

    void stop(bool drain = false);

    size_t getQueueInfo(std::chrono::system_clock::time_point &first, std::chrono::system_clock::time_point &last) const;

    bool areThreadServicingQueue() const;

private:
    std::multimap<std::chrono::system_clock::time_point , Function> task_queue_;
    std::condition_variable new_task_scheduled_;
    mutable std::mutex new_task_mutex_;
    int nthreads_servicing_queue_;
    bool stop_requested_;
    bool stop_when_empty_;
    bool shouldStop() const { return stop_requested_ || (stop_when_empty_ && task_queue_.empty()); }
};

#endif // CSCHEDULER_H
