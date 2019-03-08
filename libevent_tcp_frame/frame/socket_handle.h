#ifndef _SOCKET_HANDLE_H_
#define _SOCKET_HANDLE_H_

#include <string>
#include <sys/socket.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "packet_model.h"
#include <event2/event.h>
#include "log.h"
#include "protocol.h"
#include "deal_model.h"

using namespace std;

class IDealModel;
class CSocketHandle;

class CSocketManage
{
public:
    CSocketManage() {}
    virtual ~CSocketManage() {}

    boost::shared_ptr<CSocketHandle> GetSocketHandleByFd(int fd)
    {
        if(m_mSocketHandle.find(fd) == m_mSocketHandle.end())
        {
            return boost::shared_ptr<CSocketHandle>();
        }
        return m_mSocketHandle[fd];
    }

    void InsertSocketHandle(int sock, boost::shared_ptr<CSocketHandle> &pSocketHandle)
    {
        m_mSocketHandle[sock] = pSocketHandle;
    }

    void EraseSocketHandleBySocket(int sock)
    {
        m_mSocketHandle.erase(sock);
    }

    const boost::shared_ptr<IProtocol> &GetIProtocol() { return m_pProtocol; }
    const boost::shared_ptr<IDealModel> &GetIDealModel() { return m_pDealModel; }
protected:
    boost::shared_ptr<IProtocol> m_pProtocol;
    boost::shared_ptr<IDealModel> m_pDealModel;
    std::map<int, boost::shared_ptr<CSocketHandle>> m_mSocketHandle;
};


class CSocketHandle : public boost::enable_shared_from_this<CSocketHandle>
{
public:
    CSocketHandle(int sock, const string &strIp, boost::shared_ptr<CSocketManage> pSocketManage);

    virtual ~CSocketHandle()
    {
        ErrorOrCloseFd();
    }

    void SetReadEvent(boost::shared_ptr<struct event> &pEvent);
    void SetWriteEvent(boost::shared_ptr<struct event> &pEvent);
    void ErrorOrCloseFd();
    void ReadData();
    void WriteData();

    void ReadPacket();
    void WritePacket(boost::shared_ptr<IPacketModel> &pOutPacketModel);

private:
    int m_iSocketFd;
    string m_strIp;
    boost::shared_ptr<struct event> m_pReadEvent;
    boost::shared_ptr<struct event> m_pWriteEvent;

#ifdef EVBUFFER
    boost::shared_ptr<struct evbuffer> m_vReadBuffer;
    boost::shared_ptr<struct evbuffer> m_vWriterBuffer;
#else
    vector<char> m_vReadBuffer;
    vector<char> m_vWriterBuffer;
#endif


    boost::weak_ptr<CSocketManage> m_pSocketManage;
};

#endif

