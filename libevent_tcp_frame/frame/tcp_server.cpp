#include <event2/event.h>

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

    return true;
}

void CTcpServer::EvConnListenerCb(struct evconnlistener * stener, 
            evutil_socket_t sock, 
            struct sockaddr* addr,
            int len,
            void * ptr)
{
    CTcpServer *pTcpServer = static_cast<CTcpServer*>(ptr);
    DEBUG("new socket:" + to_string(sock));

    string strClientIp = "";
    boost::shared_ptr<CSocketHandle> pSocketHandle(new(std::nothrow)CSocketHandle(sock, strClientIp));
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
                CTcpServer::SocketReadCb, static_cast<void*>(pTcpServer) ))
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
                CTcpServer::SocketWriteCb, static_cast<void*>(pTcpServer) ))
    {
        ERROR("event_assign error");
        return;
    }
    pSocketHandle->SetReadEvent(pReadEvent);
    pSocketHandle->SetWriteEvent(pWriteEvent);

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
        char buff[5];
        int len = 0;
        while((len = recv(fd, &buff, sizeof(buff), 0)) > 0)
        {
            pSocketHandle->WriteBuffer(buff, len);
        }
        if(len < 0)
        {
            if(errno==EAGAIN || errno==EWOULDBLOCK || errno==EINTR)
            {
                pSocketHandle->ConsumeData();
            }
            else 
            {
                pSocketHandle->ErrorOrCloseFd();
                pTcpServer->EraseSocketHandleBySocket(fd);
            }
        }
        else if(0 == len)
        {
            pSocketHandle->ErrorOrCloseFd();
            pTcpServer->EraseSocketHandleBySocket(fd);
        }
        else 
        {
            pSocketHandle->ConsumeData();
        }
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
    //if(EV_WRITE == events)
    //{
    //    char buff[2048];
    //    int len = 0;
    //    for(auto &it : pSocketHandle->m_vWriterBuffer)
    //    {
    //        if(len < 2048)
    //        {
    //            buff[len] = it;
    //        }
    //    }
    //    int sendLen = send(fd, &buff, len, 0);
    //    pSocketHandle->m_vWriterBuffer.erase(pSocketHandle->m_vWriterBuffer.begin(), pSocketHandle->m_vWriterBuffer.begin()+sendLen);
    //    if(!pSocketHandle->m_vWriterBuffer.empty())
    //    {
    //        if(0 != event_add(pSocketHandle->m_pWriteEvent.get(), NULL))
    //        {
    //            ERROR("event_add error");
    //        }
    //    }
    //}
    return;
}

void CSocketHandle::WriteBuffer(char *buffer, int len)
{
    m_vReadBuffer.insert(m_vReadBuffer.end(), buffer, (buffer+(len*(sizeof(char))) ));
}

void CSocketHandle::ConsumeData()
{
    if(NULL == m_pPacketModel)
    {
        ERROR("m_pPacketModel is NULL");
        return;
    }
    while(!m_pPacketModel->ReadPacket(m_vReadBuffer))
    {
        ;
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
