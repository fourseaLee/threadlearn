#include <thread>
#include <future>
#include <vector>
#include <condition_variable>
#include "run.h"
#include "workqueue.h"

static std::thread threadServer;
static std::future<bool> threadResult;
static std::vector<std::thread> g_thread_http_workers;
static WorkQueue<Closure>* workQueue = nullptr;

//! libevent event loop
static struct Base* eventBase = nullptr;
//! HTTP server
static struct Server* eventServer = nullptr;

/** Simple wrapper to set thread name and run work queue */
static void WorkQueueRun(WorkQueue<Closure>* queue)
{
    //RenameThread("bitcoin-httpworker");
    queue->Run();
}


bool ThreadServer(Base *base, Server *server)
{
    //RenameThread("bitcoin-http");
    //LogPrint(BCLog::HTTP, "Entering http event loop\n");
    //event_base_dispatch(base);
    // Event loop will be interrupted by InterruptHTTPServer()
    // LogPrint(BCLog::HTTP, "Exited http event loop\n");
    if (base == nullptr || server == nullptr)
        return true;
    return true;
    //return event_base_got_break(base) == 0;
}

bool StartServer()
{

    int rpcThreads = std::thread::hardware_concurrency();
    std::packaged_task<bool(Base*, Server*)> task(ThreadServer);
    threadResult = task.get_future();
    threadServer= std::thread(std::move(task), eventBase, eventServer);

    for (int i = 0; i < rpcThreads; i++) {
        g_thread_http_workers.emplace_back(WorkQueueRun, workQueue);
    }
    return true;
}

void InterruptHTTPServer()
{
    /*  LogPrint(BCLog::HTTP, "Interrupting HTTP server\n");
    if (eventHTTP) {
        // Unlisten sockets
        for (evhttp_bound_socket *socket : boundSockets) {
            evhttp_del_accept_socket(eventHTTP, socket);
        }
        // Reject requests on current connections
        evhttp_set_gencb(eventHTTP, http_reject_request_cb, nullptr);
    }
    if (workQueue)
        workQueue->Interrupt();*/
}

void StopHTTPServer()
{
    /*
    LogPrint(BCLog::HTTP, "Stopping HTTP server\n");
    if (workQueue) {
        LogPrint(BCLog::HTTP, "Waiting for HTTP worker threads to exit\n");
        for (auto& thread: g_thread_http_workers) {
            thread.join();
        }
        g_thread_http_workers.clear();
        delete workQueue;
        workQueue = nullptr;
    }
    if (eventBase) {
        LogPrint(BCLog::HTTP, "Waiting for HTTP event thread to exit\n");
        // Exit the event loop as soon as there are no active events.
        event_base_loopexit(eventBase, nullptr);
        // Give event loop a few seconds to exit (to send back last RPC responses), then break it
        // Before this was solved with event_base_loopexit, but that didn't work as expected in
        // at least libevent 2.0.21 and always introduced a delay. In libevent
        // master that appears to be solved, so in the future that solution
        // could be used again (if desirable).
        // (see discussion in https://github.com/bitcoin/bitcoin/pull/6990)
        if (threadResult.valid() && threadResult.wait_for(std::chrono::milliseconds(2000)) == std::future_status::timeout) {
            LogPrintf("HTTP event loop did not exit within allotted time, sending loopbreak\n");
            event_base_loopbreak(eventBase);
        }
        threadHTTP.join();
    }
    if (eventHTTP) {
        evhttp_free(eventHTTP);
        eventHTTP = nullptr;
    }
    if (eventBase) {
        event_base_free(eventBase);
        eventBase = nullptr;
    }
    LogPrint(BCLog::HTTP, "Stopped HTTP server\n");
    */
}
