#include "socket_handle.h"
#include "log.h"
#include "main_conf.h"

CSocketHandle::CSocketHandle(int sock, const string &strIp, 
        boost::shared_ptr<CSocketManage> pSocketManage)
    : m_iSocketFd(sock), m_strIp(strIp), m_pSocketManage(pSocketManage)
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
    int len = 0;
#ifdef EVBUFFER
    len = evbuffer_read(m_vReadBuffer.get(), m_iSocketFd, -1);
    //while((len = recv(m_iSocketFd, &buff, sizeof(buff), 0)) > 0)
    //{
    //    if(0 != evbuffer_add(m_vReadBuffer.get(), buff, len))
    //    {
    //        ERROR("evbuffer_add error");
    //        return ;
    //    }
    //}
#else
    char buff[5];
    while((len = recv(m_iSocketFd, &buff, sizeof(buff), 0)) > 0)
    {
        m_vReadBuffer.insert(m_vReadBuffer.end(), buff, (buff+(len*(sizeof(char))) ));
    }
#endif
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
            boost::shared_ptr<CSocketManage> pSocketManage = m_pSocketManage.lock();
            pSocketManage->EraseSocketHandleBySocket(m_iSocketFd);
        }
    }
    else if(0 == len)
    {
        DEBUG(to_string(m_iSocketFd) + " error");
        ErrorOrCloseFd();
        boost::shared_ptr<CSocketManage> pSocketManage = m_pSocketManage.lock();
        pSocketManage->EraseSocketHandleBySocket(m_iSocketFd);
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
    DEBUG("socketId:" + to_string(m_iSocketFd) + " WriteData Len:" + to_string(len));
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
    DEBUG("socketId:" + to_string(m_iSocketFd) + " WriteData Len:" + to_string(len));
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
    boost::shared_ptr<CSocketManage> pSocketManage = m_pSocketManage.lock();
    do 
    {
        boost::shared_ptr<IPacketModel> pPacketModel(CMainConf::GetInPacketModel());
        if(!pSocketManage->GetIProtocol()->Unpacking(m_vReadBuffer, pPacketModel.get()))
        {
            break;
        }
        boost::shared_ptr<CSocketHandle> pSocketHandle = shared_from_this(); 
        pSocketManage->GetIDealModel()->DealPacket(pSocketHandle, pPacketModel);
    }while(true);
}

void CSocketHandle::WritePacket(boost::shared_ptr<IPacketModel> &pOutPacketModel)
{
    boost::shared_ptr<CSocketManage> pSocketManage = m_pSocketManage.lock();
    pSocketManage->GetIProtocol()->Packets(m_vWriterBuffer, pOutPacketModel.get());

    if(0 != event_add(m_pWriteEvent.get(), NULL))
    {
        ERROR("event_add error");
    }
}
