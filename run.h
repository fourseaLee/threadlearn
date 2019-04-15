#ifndef RUN_H
#define RUN_H

struct Base
{
    std::string data;
};

struct Server
{
    std::string func_name;
};

bool ThreadServer(Base* base, Server* server);

bool StartServer();

void InterruptHTTPServer();

void StopHTTPServer();



#endif // RUN_H
