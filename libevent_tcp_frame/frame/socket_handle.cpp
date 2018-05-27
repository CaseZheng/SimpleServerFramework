#include "socket_handle.h"
#include "log.h"
#include "main_conf.h"

CSocketHandle::CSocketHandle(int sock, const string &strClientIp)
    : m_iSocketFd(sock), m_strClientIp(strClientIp)
{
    m_pPacketModel.reset(static_cast<IPacketModel*>(CMainConf::GetPacketModel()));
}


void CSocketHandle::ErrorOrCloseFd()
{
    DEBUG("ErrorOrCloseFd");
    event_del(m_pReadEvent.get());
    event_del(m_pWriteEvent.get());
    shutdown(m_iSocketFd, SHUT_RDWR);
}

void CSocketHandle::SetWriteEvent(boost::shared_ptr<struct event> &pEvent)
{
    m_pWriteEvent = pEvent;
}

void CSocketHandle::SetReadEvent(boost::shared_ptr<struct event> &pEvent)
{
    m_pReadEvent = pEvent;
    if(0 != event_add(m_pReadEvent.get(), NULL))
    {
        ERROR("event_add error");
    }
}
