#ifndef REQUEST_H
#define REQUEST_H
#include <string>
#include <functional>

struct Data
{
    std::string header;
};

class Request
{
public:
    Request();
private:
    Data* data_;
};

typedef std::function<bool(Request* req, const std::string &)> RequestHandler;
#endif // REQUEST_H
