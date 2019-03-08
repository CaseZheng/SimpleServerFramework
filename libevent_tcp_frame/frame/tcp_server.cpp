#include <event2/event.h>
#include <arpa/inet.h>

#include "configure.h"
#include "main_conf.h"
#include "tcp_server.h"

bool CTcpServer::Init()
{
    if(NULL == m_pEventBase)
    {
        ERROR("m_pEventBase is NULL");
        return false;
    }
    struct sockaddr_in sa;
    bzero(&sa, sizeof(struct sockaddr_in));
    sa.sin_family      = AF_INET;
    sa.sin_port        = htons(atoi(CConfigure::GetPort().c_str()));
    sa.sin_addr.s_addr = inet_addr(CConfigure::GetIp().c_str());

    m_pEvConnListener.reset(
            evconnlistener_new_bind(
                m_pEventBase.get(),
                CTcpServer::EvConnListenerCb,
                static_cast<void*>(this),
                LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,
                -1,
                (struct sockaddr*)&sa,
                sizeof(struct sockaddr)
                ),
            evconnlistener_free
            );
    if(NULL == m_pEvConnListener)
    {
        ERROR("m_pEvConnListener is NULL");
        return false;
    }

    //if(0 != evconnlistener_enable(m_pEvConnListener.get()))
    //{
    //    ERROR("evconnlistener_eable failure");
    //    return false;
    //}
    
    m_pProtocol.reset(CMainConf::GetProtocol());
    if(NULL == m_pProtocol)
    {
        ERROR("m_pProtocol is NULL");
        return false;
    }

    m_pDealModel.reset(CMainConf::GetDealModel());
    if(NULL == m_pDealModel)
    {
        ERROR("m_pDealModel is NULL");
        return false;
    }

    return true;
}

void CTcpServer::EvConnListenerCb(struct evconnlistener * stener, 
            evutil_socket_t sock, 
            struct sockaddr* addr,
            int len,
            void * ptr)
{
    boost::shared_ptr<CTcpServer> pTcpServer = static_cast<CTcpServer*>(ptr)->shared_from_this();
    DEBUG("new socket:" + to_string(sock));

    string strClientIp = "";
    boost::shared_ptr<CSocketHandle> pSocketHandle(new(std::nothrow)CSocketHandle(
                                                            sock, strClientIp, pTcpServer));
    if(NULL == pSocketHandle)
    {
        ERROR("pSocketHandle is NULl");
        return;
    }

    boost::shared_ptr<struct event> pReadEvent(event_new(NULL, -1, 0, NULL, NULL), event_free);
    if(NULL == pReadEvent)
    {
        ERROR("pReadEvent is NULl");
        return;
    }
    if(0 != event_assign(pReadEvent.get(), pTcpServer->GetEventBase(), sock, EV_READ|EV_PERSIST,
                CTcpServer::SocketReadCb, static_cast<void*>(pTcpServer.get()) ))
    {
        ERROR("event_assign error");
        return;
    }

    boost::shared_ptr<struct event> pWriteEvent(event_new(NULL, -1, 0, NULL, NULL), event_free);
    if(NULL == pWriteEvent)
    {
        ERROR("pWriteEvent is NULl");
        return;
    }

    if(0 != event_assign(pWriteEvent.get(), pTcpServer->GetEventBase(), sock, EV_WRITE, 
                CTcpServer::SocketWriteCb, static_cast<void*>(pTcpServer.get()) ))
    {
        ERROR("event_assign error");
        return;
    }
    pSocketHandle->SetReadEvent(pReadEvent);
    pSocketHandle->SetWriteEvent(pWriteEvent);

    pTcpServer->InsertSocketHandle(sock, pSocketHandle);
}


void CTcpServer::SocketReadCb(int fd, short events, void *arg)
{
    CTcpServer *pTcpServer = static_cast<CTcpServer*>(arg);
    if(NULL == pTcpServer)
    {
        ERROR("pTcpServer is NULL");
        return ;
    }
    boost::shared_ptr<CSocketHandle> pSocketHandle = pTcpServer->GetSocketHandleByFd(fd);
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

void CTcpServer::SocketWriteCb(int fd, short events, void *arg)
{
    CTcpServer *pTcpServer = static_cast<CTcpServer*>(arg);
    if(NULL == pTcpServer)
    {
        ERROR("pTcpServer is NULL");
        return ;
    }
    boost::shared_ptr<CSocketHandle> pSocketHandle = pTcpServer->GetSocketHandleByFd(fd);

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
