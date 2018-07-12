#include <boost/shared_ptr.hpp>
#include "libevent_rpc_frame.h"
#include "log.h"

bool CLibeventRpcFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(!CLibeventFrame::Init(strServerName, strConfPath))
    {
        ERROR("Function failure CLibeventFrame::Init");
        return false;
    }

    m_pRpcServer.reset(new CRpcServer(m_pEventBase));
    if(NULL == m_pRpcServer)
    {
        ERROR("CRpcServer new failure");
        return false;
    }

    if(!m_pRpcServer->Init())
    {
        ERROR("RpcServer Init failure");
        return false;
    }
    DEBUG("RpcServer Init success");

    return true;
}
