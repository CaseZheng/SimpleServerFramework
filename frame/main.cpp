#include <boost/shared_ptr.hpp>

#include "main_conf.h"
#include "frame.h"
#include "log.h"

using namespace std;
using boost::shared_ptr;

int main()
{
    string strServerName;
    string strConfPath;
    if(!CMainConf::InitConf(strServerName, strConfPath))
    {
        std::cerr << "Function failed, CMainConf::InitConf" << std::endl;
        return -1;
    }

    boost::shared_ptr<CFrame> pFrame(CMainConf::GetFrame());
    if(NULL == pFrame)
    {
        std::cerr << "Function failed, CMainConf::GetFrame" << std::endl;
        return -1;
    }
    if(!pFrame->Init(strServerName, strConfPath))
    {
        std::cerr << "Function failed, pFrame->Init" << std::endl;
        return -1;
    }

    if(!pFrame->Run())
    {
        ERROR("Function failed, pFrame->Run");
        return -1;
    }

    return 0;
}
