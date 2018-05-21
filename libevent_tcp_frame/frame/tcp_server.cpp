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
}
