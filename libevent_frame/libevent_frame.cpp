#include "libevent_frame.h"
#include "log.h"

bool CLibeventFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(!CFrame::Init(strServerName, strConfPath))
    {
        ERROR("Function failure CFrame::Init");
        return false;
    }

    event_enable_debug_logging(EVENT_DBG_ALL);
    //event_enable_debug_logging(EVENT_DBG_NONE);
    event_set_log_callback(CLibeventFrame::LibeventLog);

    m_pEventBase.reset(event_base_new(), event_base_free);
    if(NULL == m_pEventBase)
    {
        ERROR("event_base_new failure");
        return false;
    }
    DEBUG("event_base_new success");

    return true;
}

bool CLibeventFrame::Run()
{
    if(!CFrame::Run())
    {
        ERROR("Function failure CFrame::Run");
        return false;
    }

    if(0 != event_base_dispatch(m_pEventBase.get()))
    {
        ERROR("event_base_dispatch failure");
        return false;
    }
    return true;
}

void CLibeventFrame::LibeventLog(int severity, const char *msg)
{
    if(EVENT_LOG_DEBUG == severity || EVENT_LOG_MSG == severity)
    {
        DEBUG(msg);
    }
    else if(EVENT_LOG_WARN == severity)
    {
        WARNING(msg);
    }
    else
    {
        ERROR(msg);
    }
}
