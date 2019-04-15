#ifndef THREADDATA_H
#define THREADDATA_H
#include "util.h"
#include <string>
#include <mutex>

class threadData
{
public:
    threadData()
    {}

    friend void threadSafeSwap(threadData &lhs, threadData &rhs)
    {

    }
private:
    std::string atomic_data_;
};

#endif // THREADDATA_H
