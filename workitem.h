#ifndef WORKITEM_H
#define WORKITEM_H
#include "closure.h"
#include "request.h"
#include <memory>

class WorkItem final: Closure
{
public:
    WorkItem(std::unique_ptr<Request> req, const std::string& header, const RequestHandler& fun ):
    req_(std::move(req)),header_(header), fun_(fun)
    {}

    void operator ()() override
    {
        fun_(req_.get(), header_);
    }

    std::unique_ptr<Request> req_;
private:
   std::string header_;
   RequestHandler fun_;

};

#endif // WORKITEM_H
