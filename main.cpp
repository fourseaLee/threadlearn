#include <QCoreApplication>
#include "threadsafequeue.h"
#include <future>
#include <functional>
#include <iostream>
#include <ctime>
#include "run.h"
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


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << std::thread::hardware_concurrency() << std::endl;

    //StartServer();
    clock_t start = clock();

    for (int i =0; i< 8; i++)
    {
        std::string hash_value = hashTask("test" + std::to_string(i),10000000);
        std::cout  << hash_value << std::endl;
    }
    clock_t center = clock();
    std::cout <<  "no mutil thread has use :" << (double)(center - start) / CLOCKS_PER_SEC  << std::endl;

    std::future<std::string>  hash_data = std::async(hashTask,"test0",10000000);

    std::future<std::string>  hash_data0 = std::async(hashTask,"test1",10000000);
    std::future<std::string>  hash_data1 = std::async(hashTask,"test2",10000000);

    std::future<std::string>  hash_data2 = std::async(hashTask,"test3",10000000);
    std::future<std::string>  hash_data3 = std::async(hashTask,"test4",10000000);
    std::future<std::string>  hash_data4 = std::async(hashTask,"test5",10000000);
    std::future<std::string>  hash_data5 = std::async(hashTask,"test6",10000000);
    std::future<std::string>  hash_data6 = std::async(hashTask,"test7",10000000);

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
    return a.exec();
}
