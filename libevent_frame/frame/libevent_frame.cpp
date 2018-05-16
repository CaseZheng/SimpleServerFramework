#include <boost/shared_ptr.hpp>
#include "libevent_frame.h"
#include "log.h"

bool CLibeventFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(!CFrame::Init(strServerName, strConfPath))
    {
        ERROR("Function failure CFrame::Init");
        return false;
    }

    m_pEventBase.reset(event_base_new(), event_base_free);
    if(NULL == m_pEventBase)
    {
        ERROR("event_base_new failure");
        return false;
    }

    m_pHttpServer.reset(new CHttpServer(m_pEventBase));
    if(NULL == m_pHttpServer)
    {
        ERROR("CHttpServer new failure");
        return false;
    }

    if(!m_pHttpServer->Init())
    {
        ERROR("HttpServer Init failure");
        return false;
    }

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
