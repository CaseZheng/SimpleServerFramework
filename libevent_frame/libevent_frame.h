#ifndef _LIBEVENT_FRAME_H_
#define _LIBEVENT_FRAME_H_ 

#include <boost/shared_ptr.hpp>
#include <event2/event.h>
#include <evhttp.h>
#include <event2/buffer.h>
#include <string>

#include "frame.h"

using namespace std;

class CLibeventFrame : public CFrame
{
public:
    CLibeventFrame(){}
    virtual ~CLibeventFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath);
    virtual bool Run();

protected:
    boost::shared_ptr<struct event_base> m_pEventBase;

private:
    static void LibeventLog(int severity, const char *msg);
};

#endif
