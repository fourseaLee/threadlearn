#ifndef CCLOCK_H
#define CCLOCK_H
#include <chrono>

class CClock
{
public:
    CClock();
    ~CClock();
    void start();
    void reset();
    uint64_t getSec();
    uint64_t getMillSec();

    void coutSec();
    void coutMillSec();
private:
    std::chrono::time_point<std::chrono::system_clock> start_;

};

#endif // CCLOCK_H
