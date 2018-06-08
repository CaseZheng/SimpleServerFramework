#ifndef _SOCKET_HANDLE_H_
#define _SOCKET_HANDLE_H_

#include <string>
#include <sys/socket.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "tcp_server.h"
#include "packet_model.h"
#include <event2/event.h>
#include "log.h"

class CTcpServer;

using namespace std;

class CSocketHandle : public boost::enable_shared_from_this<CSocketHandle>
{
public:
    CSocketHandle(int sock, const string &strClientIp, boost::shared_ptr<CTcpServer> &pTcpServer);

    virtual ~CSocketHandle()
    {
        ErrorOrCloseFd();
    }

    void SetBufferEvent(boost::shared_ptr<struct bufferevent> &pBufferEvent);
    void ErrorOrCloseFd();
    void ReadData();
    void WriteData();

    void ReadPacket();
    void WritePacket(boost::shared_ptr<IPacketModel> &pOutPacketModel);

private:
    int m_iSocketFd;
    string m_strClientIp;

    boost::shared_ptr<struct bufferevent> m_pBufferEvent;
    boost::weak_ptr<CTcpServer> m_pTcpServer;
};

#endif

