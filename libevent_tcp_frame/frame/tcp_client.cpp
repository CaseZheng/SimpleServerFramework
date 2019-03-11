#include <event2/event.h>
#include <arpa/inet.h>
#include <fcntl.h> 

#include "configure.h"
#include "main_conf.h"
#include "tcp_client.h"

bool CTcpClient::Init()
{
    if(NULL == m_pEventBase)
    {
        ERROR("m_pEventBase is NULL");
        return false;
    }
    m_pProtocol.reset(CMainConf::GetProtocol());
    if(NULL == m_pProtocol)
    {
        ERROR("m_pProtocol is NULL");
        return false;
    }

    m_pDealModel.reset(CMainConf::GetClientDealModel());
    if(NULL == m_pDealModel)
    {
        ERROR("m_pDealModel is NULL");
        return false;
    }

    return true;
}

bool CTcpClient::CreateConnection(const string& strHost, int iPort, int &iSock)
{
	if(strHost.empty() || iPort<=0)
	{
        ERROR("strHost is empty or iPort<=0");
        return false;
	}
    struct sockaddr_in server_address;
    bzero( &server_address, sizeof( server_address ) );
    server_address.sin_family = AF_INET;
    inet_pton( AF_INET, strHost.c_str(), &server_address.sin_addr );
    server_address.sin_port = htons(iPort);

    iSock = socket( PF_INET, SOCK_STREAM, 0 );

    fcntl(iSock, F_SETFL, (fcntl(iSock, F_GETFL) | O_NONBLOCK));
    int ret = connect(iSock, (struct sockaddr*)&server_address, sizeof(server_address));
    if(0==ret || (ret<0 && EINPROGRESS==errno))
    {
        DEBUG("connect " + strHost + ":" + to_string(iPort));
    }
    else
    {
        ERROR("connect " + strHost + ":" + to_string(iPort) + " failure ret:" + to_string(ret) + " errno:" + to_string(errno));
        return false;
    }

    boost::shared_ptr<CSocketHandle> pSocketHandle(new(std::nothrow)CSocketHandle(
                                                            iSock, strHost, boost::dynamic_pointer_cast<CSocketManage>(shared_from_this())));
    if(NULL == pSocketHandle)
    {
        ERROR("pSocketHandle is NULl");
        return false;
    }

    boost::shared_ptr<struct event> pReadEvent(event_new(NULL, -1, 0, NULL, NULL), event_free);
    if(NULL == pReadEvent)
    {
        ERROR("pReadEvent is NULl");
        return false;
    }
    if(0 != event_assign(pReadEvent.get(), m_pEventBase.get(), iSock, EV_READ|EV_PERSIST,
                CTcpClient::SocketReadCb, static_cast<void*>(this) ))
    {
        ERROR("event_assign error");
        return false;
    }

    boost::shared_ptr<struct event> pWriteEvent(event_new(NULL, -1, 0, NULL, NULL), event_free);
    if(NULL == pWriteEvent)
    {
        ERROR("pWriteEvent is NULl");
        return false;
    }

    if(0 != event_assign(pWriteEvent.get(), m_pEventBase.get(), iSock, EV_WRITE, 
                CTcpClient::SocketWriteCb, static_cast<void*>(this) ))
    {
        ERROR("event_assign error");
        return false;
    }
    pSocketHandle->SetReadEvent(pReadEvent);
    pSocketHandle->SetWriteEvent(pWriteEvent);

    InsertSocketHandle(iSock, pSocketHandle);

    return true;
}

void CTcpClient::SocketReadCb(int fd, short events, void *arg)
{
    CTcpClient *pTcpClient = static_cast<CTcpClient*>(arg);
    if(NULL == pTcpClient)
    {
        ERROR("pTcpClient is NULL");
        return ;
    }
    boost::shared_ptr<CSocketHandle> pSocketHandle = pTcpClient->GetSocketHandleByFd(fd);
    if(NULL == pSocketHandle)
    {
        ERROR("pSocketHandle is NULL");
        return ;
    }
    if(EV_READ == events)
    {
        pSocketHandle->ReadData();
    }
}

void CTcpClient::SocketWriteCb(int fd, short events, void *arg)
{
    CTcpClient *pTcpClient = static_cast<CTcpClient*>(arg);
    if(NULL == pTcpClient)
    {
        ERROR("pTcpClient is NULL");
        return ;
    }
    boost::shared_ptr<CSocketHandle> pSocketHandle = pTcpClient->GetSocketHandleByFd(fd);

    if(NULL == pSocketHandle)
    {
        ERROR("pSocketHandle is NULL");
        return ;
    }
    if(EV_WRITE == events)
    {
        pSocketHandle->WriteData();
    }
}
