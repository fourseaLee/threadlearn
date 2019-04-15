#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>


template<typename T>
class threadsafeQueue
{
public:
    threadsafeQueue()
    {}
    threadsafeQueue(threadsafeQueue const & other)
    {
        std::lock_guard<std::mutex> lock(other.mut);
        data_queue_ = other.data_queue_;
    }

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(mut_);
        data_queue_.push(new_value);
        data_cond_.notify_one();
    }

    void waitAndPop(T &value)
    {
        std::unique_lock<std::mutex> lock(mut_);
        data_cond_.wait(lock, [this]{return !data_queue_.empty();});
        std::shared_ptr<T> res (std::make_shared<T> (data_queue_.front()));
        data_queue_.pop();
        return res;
    }

    bool tryPop(T& value)
    {
        std::lock_guard<std::mutex> lock(mut_);
        if (data_queue_.empty())
            return false;
        value = data_queue_.front();
        data_queue_.pop();
        return true;
    }

    std::shared_ptr<T> tryPop()
    {
        std::lock_guard<std::mutex> lock(mut_);
        if(data_queue_.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue_.front()));
        data_queue_.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mut_);
        return data_queue_.empty();

    }




private:
    mutable std::mutex mut_;
    std::queue<T> data_queue_;
    std::condition_variable data_cond_;



};

#endif // THREADSAFEQUEUE_H
