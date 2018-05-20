#include <boost/shared_ptr.hpp>
#include "libevent_http_frame.h"
#include "log.h"

bool CLibeventHttpFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(!CLibeventFrame::Init(strServerName, strConfPath))
    {
        ERROR("Function failure CLibeventFrame::Init");
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
    DEBUG("HttpServer Init success");

    return true;
}
