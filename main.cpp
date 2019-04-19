//#include <QCoreApplication>
#include "threadsafequeue.h"
#include <future>
#include <functional>
#include <iostream>
#include <ctime>
#include "run.h"
#include "cscheduler.h"
#include "cclock.h"

CScheduler scheduler;
CClock s_clock;
std::string hashTask(std::string data, uint64_t hash_cout)
{
   std::string iter_data = data;
   std::hash<std::string> hash_fn;
   for(uint64_t i =0; i < hash_cout; i++)
   {
       iter_data = std::to_string(hash_fn(iter_data));
   }
   return  iter_data;
}

void  hashTask1(std::string data, uint64_t hash_cout)
{
   CClock clock_tmp;
   clock_tmp.start();
   std::string iter_data = data;
   std::hash<std::string> hash_fn;
   for(uint64_t i =0; i < hash_cout; i++)
   {
       iter_data = std::to_string(hash_fn(iter_data));
   }

   std::cout <<  "one task : " << std::endl;
   clock_tmp.coutSec();
   std::cout << "hashtask1 : " << iter_data << std::endl;;
}

void testscheduler()
{
    std::vector<std::thread> vect_thread;
    for (int i =0; i< 4; i++)
    {
        std::thread t(&CScheduler::serviceQueue,&scheduler);
        //queue_thread.push(std::move(t));

        vect_thread.push_back(std::move(t));
    }

    for (int i =0; i< 8; i++)
    {
        std::string hash_value = "test" + std::to_string(i);
        scheduler.schedule(std::bind(hashTask1,hash_value,10000000));
        //scheduler.schedule(std::bind(&PeerLogicValidation::CheckForStaleTipAndEvictPeers, this, consensusParams), EXTRA_PEER_CHECK_INTERVAL * 1000);
    }

    scheduler.stop(true);
    for (int i =0; i< 4; i++)
    {
        vect_thread[i].join();
    }

}

int main(int argc, char *argv[])
{
   // QCoreApplication a(argc, argv);

    std::cout << std::thread::hardware_concurrency() << std::endl;

    //StartServer();
    s_clock.start();

    for (int i =0; i< 8; i++)
    {
        std::string hash_value = hashTask("test" + std::to_string(i),10000000);
        std::cout  << hash_value << std::endl;
    }

    std::cout << "no mutil thread :" << std::endl;
    s_clock.coutSec();
    s_clock.reset();

    std::future<std::string>  hash_data = std::async(std::launch::async,hashTask,"test0",10000000);
    std::future<std::string>  hash_data0 = std::async(std::launch::async,hashTask,"test1",10000000);
    std::future<std::string>  hash_data1 = std::async(std::launch::async,hashTask,"test2",10000000);
    std::future<std::string>  hash_data2 = std::async(std::launch::async,hashTask,"test3",10000000);
    std::future<std::string>  hash_data3 = std::async(std::launch::async,hashTask,"test4",10000000);
    std::future<std::string>  hash_data4 = std::async(std::launch::async,hashTask,"test5",10000000);
    std::future<std::string>  hash_data5 = std::async(std::launch::async,hashTask,"test6",10000000);
    std::future<std::string>  hash_data6 = std::async(std::launch::async,hashTask,"test7",10000000);

    std::cout << hash_data.get() << std::endl;
    std::cout << hash_data0.get() << std::endl;
    std::cout << hash_data1.get() << std::endl;
    std::cout << hash_data2.get() << std::endl;
    std::cout << hash_data3.get() << std::endl;
    std::cout << hash_data4.get() << std::endl;
    std::cout << hash_data5.get() << std::endl;
    std::cout << hash_data6.get() << std::endl;

    std::cout <<  "mutil thread :" << std::endl;
    s_clock.coutSec();
    s_clock.reset();

    testscheduler();
    std::cout << "mutil task :" << std::endl;
    s_clock.coutSec();

    return 0;
    //return a.exec();
}
