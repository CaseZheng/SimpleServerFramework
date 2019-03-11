#ifndef _LIBEVENT_TCP_FRAME_H_
#define _LIBEVENT_TCP_FRAME_H_ 

#include <string>

#include "frame.h"
#include "libevent_frame.h"
#include "tcp_server.h"
#include "tcp_client.h"

using namespace std;

class CLibeventTcpFrame : public CLibeventFrame
{
public:
    CLibeventTcpFrame(){}
    virtual ~CLibeventTcpFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath);

public:
    static boost::shared_ptr<CTcpServer> m_pTcpServer;
    static boost::shared_ptr<CTcpClient> m_pTcpClient;
};

#endif
