#include <boost/shared_ptr.hpp>
#include "libevent_tcp_frame.h"
#include "log.h"

bool CLibeventTcpFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(!CLibeventFrame::Init(strServerName, strConfPath))
    {
        ERROR("Function failure CLibeventFrame::Init");
        return false;
    }

    m_pTcpServer.reset(new CTcpServer(m_pEventBase));
    if(NULL == m_pTcpServer)
    {
        ERROR("CTcpServer new failure");
        return false;
    }

    if(!m_pTcpServer->Init())
    {
        ERROR("TcpServer Init failure");
        return false;
    }
    DEBUG("TcpServer Init success");

    return true;
}
