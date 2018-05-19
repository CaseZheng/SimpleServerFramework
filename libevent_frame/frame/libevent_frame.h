#ifndef _LIBEVENT_HTTP_FRAME_H_
#define _LIBEVENT_HTTP_FRAME_H_ 

#include <string>

#include "frame.h"
#include "http_server.h"
#include "timer_server.h"

using namespace std;

class CLibeventFrame : public CFrame
{
public:
    CLibeventFrame(){}
    virtual ~CLibeventFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath);
    virtual bool Run();

private:
    static void LibeventLog(int severity, const char *msg);

    boost::shared_ptr<struct event_base> m_pEventBase;
    boost::shared_ptr<CHttpServer> m_pHttpServer;
    boost::shared_ptr<CTimerServer> m_pTimerServer;
};

#endif
