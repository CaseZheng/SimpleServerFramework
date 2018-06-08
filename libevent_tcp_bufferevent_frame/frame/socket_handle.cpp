#include "socket_handle.h"
#include "log.h"
#include "main_conf.h"

CSocketHandle::CSocketHandle(int sock, const string &strClientIp, 
        boost::shared_ptr<CTcpServer> &pTcpServer)
    : m_iSocketFd(sock), m_strClientIp(strClientIp), m_pTcpServer(pTcpServer)
{
}


void CSocketHandle::ErrorOrCloseFd()
{
    DEBUG("ErrorOrCloseFd");
}

void CSocketHandle::SetBufferEvent(boost::shared_ptr<struct bufferevent> &pBufferEvent)
{
    bufferevent_enable(pBufferEvent.get(), EV_READ|EV_PERSIST);
    m_pBufferEvent = pBufferEvent;
}

void CSocketHandle::ReadData()
{
    struct evbuffer *pEvbuffer = bufferevent_get_input(m_pBufferEvent.get());
    int len = 0;
    len = evbuffer_read(pEvbuffer, m_iSocketFd, -1);
    if(len < 0)
    {
        if(errno==EAGAIN || errno==EWOULDBLOCK || errno==EINTR)
        {
            ReadPacket();
        }
        else 
        {
            DEBUG(to_string(m_iSocketFd) + " error");
            ErrorOrCloseFd();
            boost::shared_ptr<CTcpServer> pTcpServer = m_pTcpServer.lock();
            pTcpServer->EraseSocketHandleBySocket(m_iSocketFd);
        }
    }
    else if(0 == len)
    {
        DEBUG(to_string(m_iSocketFd) + " error");
        ErrorOrCloseFd();
        boost::shared_ptr<CTcpServer> pTcpServer = m_pTcpServer.lock();
        pTcpServer->EraseSocketHandleBySocket(m_iSocketFd);
    }
    else 
    {
        ReadPacket();
    }
}

void CSocketHandle::WriteData()
{
    struct evbuffer *pEvbuffer = bufferevent_get_output(m_pBufferEvent.get());
    if(0 == evbuffer_get_length(pEvbuffer)) 
    {
        DEBUG("buff is empty");
        return;
    }
    int len=0;
    if((len = evbuffer_write(pEvbuffer, m_iSocketFd)) < 0)
    {
        ERROR("send data error");
        ErrorOrCloseFd();
        return;
    }
    DEBUG(to_string(len));
    if(0 != evbuffer_get_length(pEvbuffer))
    {
        bufferevent_enable(m_pBufferEvent.get(), EV_WRITE);
    }
}

void CSocketHandle::ReadPacket()
{
    boost::shared_ptr<CTcpServer> pTcpServer = m_pTcpServer.lock();
    struct evbuffer *pEvbuffer = bufferevent_get_input(m_pBufferEvent.get());
    do 
    {
        boost::shared_ptr<IPacketModel> pPacketModel(CMainConf::GetInPacketModel());
        if(!pTcpServer->GetIProtocol()->Unpacking(pEvbuffer, pPacketModel.get()))
        {
            break;
        }
        boost::shared_ptr<CSocketHandle> pSocketHandle = shared_from_this(); 
        pTcpServer->GetIDealModel()->DealPacket(pSocketHandle, pPacketModel);
    }while(true);
}

void CSocketHandle::WritePacket(boost::shared_ptr<IPacketModel> &pOutPacketModel)
{
    boost::shared_ptr<CTcpServer> pTcpServer = m_pTcpServer.lock();
    struct evbuffer *pEvbuffer = bufferevent_get_output(m_pBufferEvent.get());
    pTcpServer->GetIProtocol()->Packets(pEvbuffer, pOutPacketModel.get());

    bufferevent_enable(m_pBufferEvent.get(), EV_WRITE);
}
