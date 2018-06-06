#include "socket_handle.h"
#include "log.h"
#include "main_conf.h"

CSocketHandle::CSocketHandle(int sock, const string &strClientIp, 
        boost::shared_ptr<CTcpServer> &pTcpServer)
    : m_iSocketFd(sock), m_strClientIp(strClientIp), m_pTcpServer(pTcpServer)
{
#ifdef EVBUFFER 
    m_vReadBuffer.reset(evbuffer_new(), evbuffer_free);
    m_vWriterBuffer.reset(evbuffer_new(), evbuffer_free);
#endif
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

void CSocketHandle::ReadData()
{
    char buff[5];
    int len = 0;
    while((len = recv(m_iSocketFd, &buff, sizeof(buff), 0)) > 0)
    {
#ifdef EVBUFFER
        if(0 != evbuffer_add(m_vReadBuffer.get(), buff, len))
        {
            ERROR("evbuffer_add error");
            return ;
        }
#else
        m_vReadBuffer.insert(m_vReadBuffer.end(), buff, (buff+(len*(sizeof(char))) ));
#endif
    }
    if(len < 0)
    {
        if(errno==EAGAIN || errno==EWOULDBLOCK || errno==EINTR)
        {
            ReadPacket();
        }
        else 
        {
            ErrorOrCloseFd();
            boost::shared_ptr<CTcpServer> pTcpServer = m_pTcpServer.lock();
            pTcpServer->EraseSocketHandleBySocket(m_iSocketFd);
        }
    }
    else if(0 == len)
    {
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
#ifdef EVBUFFER
    if(0 == evbuffer_get_length(m_vWriterBuffer.get()))
    {
        DEBUG("buff is empty");
        return;
    }
    int len=0;
    if((len = evbuffer_write(m_vWriterBuffer.get(), m_iSocketFd)) < 0)
    {
        ERROR("send data error");
        ErrorOrCloseFd();
        return;
    }
    DEBUG(to_string(len));
    if(0 != evbuffer_get_length(m_vWriterBuffer.get()))
    {
        if(0 != event_add(m_pWriteEvent.get(), NULL))
        {
            ERROR("event_add error");
        }
    }

#else
    if(m_vWriterBuffer.empty())
    {
        DEBUG("buff is empty");
        return;
    }
    char *buff = &m_vWriterBuffer[0];
    int len = 0;
    //发送
    len = send(m_iSocketFd, buff, m_vWriterBuffer.size(), 0);
    if(len < 0)
    {
        ERROR("send data error");
        ErrorOrCloseFd();
        return;
    }
    DEBUG(to_string(len));
    //删除已发送字节
    m_vWriterBuffer.erase(m_vWriterBuffer.begin(), m_vWriterBuffer.begin()+len*sizeof(char));

    if(!m_vWriterBuffer.empty())
    {
        if(0 != event_add(m_pWriteEvent.get(), NULL))
        {
            ERROR("event_add error");
        }
    }
#endif
}

void CSocketHandle::ReadPacket()
{
    boost::shared_ptr<CTcpServer> pTcpServer = m_pTcpServer.lock();
    do 
    {
        boost::shared_ptr<IPacketModel> pPacketModel(CMainConf::GetInPacketModel());
        if(!pTcpServer->GetIProtocol()->Unpacking(m_vReadBuffer, pPacketModel.get()))
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
    pTcpServer->GetIProtocol()->Packets(m_vWriterBuffer, pOutPacketModel.get());

    if(0 != event_add(m_pWriteEvent.get(), NULL))
    {
        ERROR("event_add error");
    }
}
