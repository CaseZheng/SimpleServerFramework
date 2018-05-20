#ifndef _LIBEVENT_TIMER_FRAME_H_
#define _LIBEVENT_TIMER_FRAME_H_ 

#include <string>

#include "frame.h"
#include "libevent_frame.h"
#include "timer_server.h"

using namespace std;

class CLibeventTimerFrame : public CLibeventFrame
{
public:
    CLibeventTimerFrame(){}
    virtual ~CLibeventTimerFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath);

private:
    boost::shared_ptr<CTimerServer> m_pTimerServer;
};

#endif
