#include <boost/shared_ptr.hpp>
#include "libevent_tcp_frame.h"
#include "log.h"

boost::shared_ptr<CTcpServer> CLibeventTcpFrame::m_pTcpServer;
boost::shared_ptr<CTcpClient> CLibeventTcpFrame::m_pTcpClient;

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

    m_pTcpClient.reset(new CTcpClient(m_pEventBase));
    if(NULL == m_pTcpClient)
    {
        ERROR("CTcpClient new failure");
        return false;
    }

    if(!m_pTcpClient->Init())
    {
        ERROR("TcpClient Init failure");
        return false;
    }
    DEBUG("TcpClient Init success");

    return true;
}
