#include <boost/shared_ptr.hpp>
#include <csignal>

#include "main_conf.h"
#include "frame.h"
#include "log.h"

using namespace std;

boost::shared_ptr<CFrame> gFrame;

void SignalHandler(int signalNum)
{
    if(SIGTERM == signalNum)
    {
        DEBUG("signalNum SIGTERM");
        if(NULL==gFrame || !gFrame->Exit())
        {
            DEBUG("gFrame is NULL");
            exit(0);
        }
    }
    else 
    {
        DEBUG("Invalid signalNum:" + to_string(signalNum));
    }
}

int main()
{
    string strServerName;
    string strConfPath;
    if(!CMainConf::InitConf(strServerName, strConfPath))
    {
        std::cerr << "Function failed, CMainConf::InitConf" << std::endl;
        return -1;
    }

    gFrame.reset(CMainConf::GetFrame());
    if(NULL == gFrame)
    {
        std::cerr << "Function failed, CMainConf::GetFrame" << std::endl;
        return -1;
    }
    if(!gFrame->Init(strServerName, strConfPath))
    {
        std::cerr << "Function failed, gFrame->Init" << std::endl;
        return -1;
    }

    //守护进程
    daemon(1, 0);

    //过滤信号
    signal(SIGALRM, SignalHandler);
    signal(SIGPIPE, SignalHandler);
    signal(SIGHUP,  SignalHandler);
    signal(SIGINT,  SignalHandler);
    signal(SIGQUIT, SignalHandler);
    signal(SIGCHLD, SignalHandler);
    signal(SIGTERM, SignalHandler);
    //signal(SIGKILL, SignalHandler); //SIGKILL信号无法不捕获

    if(!gFrame->Run())
    {
        ERROR("Function failed, gFrame->Run");
        return -1;
    }

    return 0;
}
