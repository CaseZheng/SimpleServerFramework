#ifndef _EXAMPLE_TIMER_H_
#define _EXAMPLE_TIMER_H_

#include "timer_server.h"

class CExampleTimer : public ITimerServer 
{
public:
    CExampleTimer() {}
    ~CExampleTimer() {}
private:
    virtual bool TimerDeal();
    virtual bool GetTimeval(struct timeval &tv);
};

#endif
