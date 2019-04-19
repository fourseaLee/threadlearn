//#include <QCoreApplication>
#include "threadsafequeue.h"
#include <future>
#include <functional>
#include <iostream>
#include <ctime>
#include "run.h"
#include "cscheduler.h"

CScheduler scheduler;
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
   clock_t start = clock();
   std::string iter_data = data;
   std::hash<std::string> hash_fn;
   for(uint64_t i =0; i < hash_cout; i++)
   {
       iter_data = std::to_string(hash_fn(iter_data));
   }
   clock_t end = clock();
   std::cout <<  "one task has use :" << (double)(end - start) / CLOCKS_PER_SEC  << std::endl;
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
        scheduler.schedule(std::bind(hashTask1,hash_value,1000000));
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
    clock_t start = clock();

    for (int i =0; i< 8; i++)
    {
        std::string hash_value = hashTask("test" + std::to_string(i),1000000);
        std::cout  << hash_value << std::endl;
    }
    clock_t center = clock();
    std::cout <<  "no mutil thread has use :" << (double)(center - start) / CLOCKS_PER_SEC  << std::endl;


    std::future<std::string>  hash_data = std::async(std::launch::async,hashTask,"test0",1000000);
    std::future<std::string>  hash_data0 = std::async(std::launch::async,hashTask,"test1",1000000);
    std::future<std::string>  hash_data1 = std::async(std::launch::async,hashTask,"test2",1000000);
    std::future<std::string>  hash_data2 = std::async(std::launch::async,hashTask,"test3",1000000);
    std::future<std::string>  hash_data3 = std::async(std::launch::async,hashTask,"test4",1000000);
    std::future<std::string>  hash_data4 = std::async(std::launch::async,hashTask,"test5",1000000);
    std::future<std::string>  hash_data5 = std::async(std::launch::async,hashTask,"test6",1000000);
    std::future<std::string>  hash_data6 = std::async(std::launch::async,hashTask,"test7",1000000);

    std::cout << hash_data.get() << std::endl;
    std::cout << hash_data0.get() << std::endl;
    std::cout << hash_data1.get() << std::endl;
    std::cout << hash_data2.get() << std::endl;
    std::cout << hash_data3.get() << std::endl;
    std::cout << hash_data4.get() << std::endl;
    std::cout << hash_data5.get() << std::endl;
    std::cout << hash_data6.get() << std::endl;

    clock_t end = clock();

    std::cout <<  "mutil thread has use :" << (double)(end - center) / CLOCKS_PER_SEC  << std::endl;

    testscheduler();

    clock_t endend = clock();

    std::cout <<  "mutil thread has use :" << (double)(endend - end) / CLOCKS_PER_SEC  << std::endl;



    //return a.exec();
}
