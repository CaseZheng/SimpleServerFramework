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

    boost::shared_ptr<struct bufferevent> pBufferEvent(
                        bufferevent_socket_new(pTcpServer->GetEventBase(), sock, BEV_OPT_CLOSE_ON_FREE),
                        bufferevent_free);
    bufferevent_setcb(pBufferEvent.get(), CTcpServer::SocketReadCb, 
            CTcpServer::SocketWriteCb, CTcpServer::SocketEventCb, static_cast<void*>(pTcpServer.get()));

    pSocketHandle->SetBufferEvent(pBufferEvent);

    pTcpServer->InsertSocketHandle(sock, pSocketHandle);
}

void CTcpServer::InsertSocketHandle(int sock, boost::shared_ptr<CSocketHandle> &pSocketHandle)
{
    m_mSocketHandle[sock] = pSocketHandle;
}


void CTcpServer::EraseSocketHandleBySocket(int sock)
{
    m_mSocketHandle.erase(sock);
}

void CTcpServer::SocketReadCb(struct bufferevent *bev, void *ctx)
{
    CTcpServer *pTcpServer = static_cast<CTcpServer*>(ctx);
    if(NULL == pTcpServer)
    {
        ERROR("pTcpServer is NULL");
        return ;
    }
    int fd  = bufferevent_getfd(bev);
    boost::shared_ptr<CSocketHandle> pSocketHandle = pTcpServer->GetSocketHandleByFd(fd);
    if(NULL == pSocketHandle)
    {
        ERROR("pSocketHandle is NULL");
        return ;
    }
    pSocketHandle->ReadData();
}

void CTcpServer::SocketWriteCb(struct bufferevent *bev, void *ctx)
{
    CTcpServer *pTcpServer = static_cast<CTcpServer*>(ctx);
    if(NULL == pTcpServer)
    {
        ERROR("pTcpServer is NULL");
        return ;
    }
    int fd  = bufferevent_getfd(bev);
    boost::shared_ptr<CSocketHandle> pSocketHandle = pTcpServer->GetSocketHandleByFd(fd);
    if(NULL == pSocketHandle)
    {
        ERROR("pSocketHandle is NULL");
        return ;
    }
    pSocketHandle->WriteData();
}

void CTcpServer::SocketEventCb(struct bufferevent *bev, short what, void *ctx)
{
    CTcpServer *pTcpServer = static_cast<CTcpServer*>(ctx);
    if(NULL == pTcpServer)
    {
        ERROR("pTcpServer is NULL");
        return ;
    }
    int fd  = bufferevent_getfd(bev);
    boost::shared_ptr<CSocketHandle> pSocketHandle = pTcpServer->GetSocketHandleByFd(fd);
    if(NULL == pSocketHandle)
    {
        ERROR("pSocketHandle is NULL");
        return ;
    }
    if((what & BEV_EVENT_EOF) || (what & BEV_EVENT_ERROR) || (what & BEV_EVENT_TIMEOUT))
    {
        pSocketHandle->ErrorOrCloseFd();
    }
    else 
    {
        ERROR("fd:" + to_string(fd) + " event:" + to_string(what));
    }
}

boost::shared_ptr<CSocketHandle> CTcpServer::GetSocketHandleByFd(int fd)
{
    if(m_mSocketHandle.find(fd) == m_mSocketHandle.end())
    {
        return boost::shared_ptr<CSocketHandle>();
    }
    return m_mSocketHandle[fd];
}
