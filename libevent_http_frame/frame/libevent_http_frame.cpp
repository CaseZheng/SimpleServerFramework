#include <boost/shared_ptr.hpp>
#include "libevent_http_frame.h"
#include "http_server.h"
#include "log.h"

bool CLibeventHttpFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(!CFrame::Init(strServerName, strConfPath))
    {
        ERROR("Function failure CFrame::Init");
        return false;
    }
    return true;
}
bool CLibeventHttpFrame::Run()
{
    if(!CFrame::Run())
    {
        ERROR("Function failure CFrame::Run");
        return false;
    }

    boost::shared_ptr<CHttpServer> pHttpServer(new CHttpServer);
    if(NULL == pHttpServer)
    {
        ERROR("CHttpServer new failure");
        return false;
    }

    if(!pHttpServer->Init())
    {
        ERROR("HttpServer Init failure");
        return false;
    }

    if(!pHttpServer->Run())
    {
        ERROR("HttpServer Run failure");
        return false;
    }

    return true;
}
