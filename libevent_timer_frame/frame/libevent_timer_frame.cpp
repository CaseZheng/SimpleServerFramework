#include <boost/shared_ptr.hpp>
#include "libevent_timer_frame.h"
#include "log.h"

bool CLibeventTimerFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(!CLibeventFrame::Init(strServerName, strConfPath))
    {
        ERROR("Function failure CLibeventFrame::Init");
        return false;
    }

    m_pTimerServer.reset(new CTimerServer(m_pEventBase));
    if(NULL == m_pTimerServer)
    {
        ERROR("CTimerServer new failure");
        return false;
    }

    if(!m_pTimerServer->Init())
    {
        ERROR("TimerServer Init failure");
        return false;
    }
    DEBUG("TimerServer Init success");

    return true;
}
