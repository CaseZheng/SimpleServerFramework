#ifndef _TIMER_H_ 
#define _TIMER_H_ 

#include <string>
#include <boost/shared_ptr.hpp>
#include <event2/event.h>

using namespace std;


typedef event_callback_fn EVTIMER_CB;

class CTimer 
{
public:
    CTimer(boost::shared_ptr<struct event_base> pEventBase, EVTIMER_CB cb, void* arg);
    virtual ~CTimer(){}

private:
    CTimer(const CTimer&);
    CTimer& operator=(const CTimer&);

public:
    bool AddTimer(const struct timeval &tv);
    bool DelTimer();

private:
    EVTIMER_CB m_cb;
    void *m_arg;
    boost::shared_ptr<struct event> m_pEvent;
    boost::shared_ptr<struct event_base> m_pEventBase;
};


#endif
