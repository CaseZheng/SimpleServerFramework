#ifndef _TIMER_SERVER_H_
#define _TIMER_SERVER_H_ 

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include "log.h"
#include "timer.h"

using namespace std;
using boost::shared_ptr;

class ITimerServer : public boost::noncopyable
{
public:
    ITimerServer();
    virtual ~ITimerServer() {}

    static void TimerCb(evutil_socket_t fd, short events, void * arg);
    void ResetTimer(CTimer* pTimer)
    {
        m_pTimer.reset(pTimer);
    }
    bool StartTimerServer();
private:
    virtual bool TimerCbStart();
    virtual bool TimerCbEnd();

    virtual bool GetTimeval(struct timeval &tv) = 0;
    virtual bool TimerDeal() = 0;
private:
    boost::shared_ptr<CTimer> m_pTimer;
};

class CTimerServer
{
public:
	CTimerServer(boost::shared_ptr<struct event_base> pEventBase) : m_pEventBase(pEventBase) {}
	virtual ~CTimerServer(){}

	bool Init();
    static bool InsertTimer(ITimerServer *pTimerServer);

private:
	CTimerServer(const CTimerServer&);
	CTimerServer& operator=(const CTimerServer&);

private:
    boost::shared_ptr<struct event_base> m_pEventBase;
    static vector<boost::shared_ptr<ITimerServer>> m_pTimerServer;
};

#endif
