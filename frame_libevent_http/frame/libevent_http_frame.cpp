#include "libevent_http_frame.h"
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
    if(CFrame::Run())
    {
        ERROR("Function failure CFrame::Run");
        return false;
    }
    return true;
}
