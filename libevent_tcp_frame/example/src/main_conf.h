#ifndef _MAIN_CONF_H_
#define _MAIN_CONF_H_

#include <string>
#include <iostream>
#include "libevent_frame.h"
#include "libevent_tcp_frame.h"
#include "log.h"
#include "tools.h"

using namespace std;

class CMainConf 
{
public:
    static bool InitConf(string &strServerName, string &strConfPath)
    {
        strServerName = tools::GetProcessName();
        strConfPath = "./config.xml";

        return true;
    }

    static CFrame *GetFrame()
    {
        return new CLibeventTcpFrame();
    }
};

#endif
