#ifndef THREADGROUP_H
#define THREADGROUP_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <shared_mutex>
/*
class thread_group
{
private:
    thread_group(thread_group const&);
    thread_group& operator=(thread_group const&);
public:
    thread_group() {}
    ~thread_group()
    {
        for(std::list<thread*>::iterator it=threads.begin(),end=threads.end();
            it!=end;
            ++it)
        {
            delete *it;
        }
    }

    bool is_this_thread_in()
    {

        std::thread::id id = std::this_thread::get_id();
        std::shared_lock<shared_mutex> guard(m);
        for(std::list<thread*>::iterator it=threads.begin(),end=threads.end();
            it!=end;
            ++it)
        {
            if ((*it)->get_id() == id)
                return true;
        }
        return false;
    }

    bool is_thread_in(thread* thrd)
    {
        if(thrd)
        {
            std::thread::id id = thrd->get_id();

            std::shared_lock<shared_mutex> guard(m);
            for(std::list<thread*>::iterator it=threads.begin(),end=threads.end();
                it!=end;
                ++it)
            {
                if ((*it)->get_id() == id)
                    return true;
            }
            return false;
        }
        else
        {
            return false;
        }
    }

    template<typename F>
    std::thread* create_thread(F threadfunc)
    {

        std::lock_guard<shared_mutex> guard(m);
        std::csbl::unique_ptr<std::thread> new_thread(new thread(threadfunc));
        threads.push_back(new_thread.get());
        return new_thread.release();
    }
    void add_thread(thread* thrd)
    {
        if(thrd)
        {
            BOOST_THREAD_ASSERT_PRECONDITION( ! is_thread_in(thrd) ,
                                              thread_resource_error(static_cast<int>(system::errc::resource_deadlock_would_occur), "boost::thread_group: trying to add a duplicated thread")
                                              );

            boost::lock_guard<shared_mutex> guard(m);
            threads.push_back(thrd);
        }
    }

    void remove_thread(thread* thrd)
    {
        boost::lock_guard<shared_mutex> guard(m);
        std::list<thread*>::iterator const it=std::find(threads.begin(),threads.end(),thrd);
        if(it!=threads.end())
        {
            threads.erase(it);
        }
    }

    void join_all()
    {
        BOOST_THREAD_ASSERT_PRECONDITION( ! is_this_thread_in() ,
                                          thread_resource_error(static_cast<int>(system::errc::resource_deadlock_would_occur), "boost::thread_group: trying joining itself")
                                          );
        boost::shared_lock<shared_mutex> guard(m);

        for(std::list<thread*>::iterator it=threads.begin(),end=threads.end();
            it!=end;
            ++it)
        {
            if ((*it)->joinable())
                (*it)->join();
        }
    }
    size_t size() const
    {
        boost::shared_lock<shared_mutex> guard(m);
        return threads.size();
    }

private:
    std::list<thread*> threads;
    mutable shared_mutex m;
};

*/

#endif // THREADGROUP_H
