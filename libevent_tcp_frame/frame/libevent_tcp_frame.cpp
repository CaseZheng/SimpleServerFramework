#include <boost/shared_ptr.hpp>
#include "libevent_tcp_frame.h"
#include "log.h"

extern boost::shared_ptr<CFrame> gFrame;

bool CLibeventTcpFrame::CreateClient(const string& strHost, int iPort, int &iSock)
{
    auto pTcpFrame = boost::dynamic_pointer_cast<CLibeventTcpFrame>(gFrame);
    return pTcpFrame->m_pTcpClient->CreateConnection(strHost, iPort, iSock);
}

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
