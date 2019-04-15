#ifndef CLOSURE_H
#define CLOSURE_H

class Closure
{
public:
    virtual ~ Closure()
    {}
    virtual void operator()()=0;

};

#endif // CLOSURE_H
