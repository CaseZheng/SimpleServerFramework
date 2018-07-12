#ifndef _LIBEVENT_RPC_FRAME_H_
#define _LIBEVENT_RPC_FRAME_H_ 

#include <string>

#include "libevent_frame.h"
#include "rpc_server.h"

using namespace std;

class CLibeventRpcFrame : public CLibeventFrame
{
public:
    CLibeventRpcFrame(){}
    virtual ~CLibeventRpcFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath);

private:

    boost::shared_ptr<CRpcServer> m_pRpcServer;
};

#endif
