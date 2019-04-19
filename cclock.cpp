#include "cclock.h"
#include <iostream>

CClock::CClock()
{

}

CClock::~CClock()
{

}

void CClock::start()
{

    start_ = std::chrono::system_clock::now();
}

void CClock::reset()
{
    start_ = std::chrono::system_clock::now();
}

uint64_t CClock::getSec()
{
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    uint64_t ret = std::chrono::duration_cast<std::chrono::seconds>(now - start_).count();
    return ret;
}

uint64_t CClock::getMillSec()
{
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    uint64_t ret = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count();
    return ret;
}

void CClock::coutSec()
{
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    uint64_t ret = std::chrono::duration_cast<std::chrono::seconds>(now - start_).count();
    std::cout <<  ret << std::endl;
}

void CClock::coutMillSec()
{
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    uint64_t ret = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count();
    std::cout << ret << std::endl;
}
