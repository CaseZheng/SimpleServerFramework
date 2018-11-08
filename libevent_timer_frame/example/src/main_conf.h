#ifndef _MAIN_CONF_H_
#define _MAIN_CONF_H_

#include <string>
#include <iostream>
#include "libevent_frame.h"
#include "libevent_timer_frame.h"
#include "log.h"
#include "tools.h"

#include "example_timer.h"

using namespace std;

class CMainConf 
{
public:
    static bool InitConf(string &strServerName, string &strConfPath)
    {
        strServerName = tools::GetProcessName();

        return true;
    }

    static CFrame *GetFrame()
    {
        return new CLibeventTimerFrame();
    }

    static bool InsertTimer()
    {
        if(!CTimerServer::InsertTimer(new (std::nothrow) CExampleTimer))
        {
            ERROR("InsertTimer failure");
            return false;
        }
        return true;
    }
};

#endif
