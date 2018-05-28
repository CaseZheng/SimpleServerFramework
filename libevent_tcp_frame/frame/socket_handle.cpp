#include "socket_handle.h"
#include "log.h"
#include "main_conf.h"

CSocketHandle::CSocketHandle(int sock, const string &strClientIp, 
        boost::shared_ptr<CTcpServer> &pTcpServer)
    : m_iSocketFd(sock), m_strClientIp(strClientIp), m_pTcpServer(pTcpServer)
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

void CSocketHandle::ReadData()
{
    char buff[5];
    int len = 0;
    while((len = recv(m_iSocketFd, &buff, sizeof(buff), 0)) > 0)
    {
        m_vReadBuffer.insert(m_vReadBuffer.end(), buff, (buff+(len*(sizeof(char))) ));
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
    if(m_vWriterBuffer.empty())
    {
        return;
    }
    char *buff = &m_vWriterBuffer[0];
    int len = 0;
    //发送
    len = send(m_iSocketFd, buff, m_vWriterBuffer.size(), 0);
    if(len < 0)
    {
        ErrorOrCloseFd();
        return;
    }
    //删除已发送字节
    m_vWriterBuffer.erase(m_vWriterBuffer.begin(), m_vWriterBuffer.begin()+len*sizeof(char));

    if(!m_vWriterBuffer.empty())
    {
        if(0 != event_add(m_pWriteEvent.get(), NULL))
        {
            ERROR("event_add error");
        }
    }
}

void CSocketHandle::ReadPacket()
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

void CSocketHandle::WritePacket()
{
}
