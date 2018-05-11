#ifndef _MAIN_CONF_H_
#define _MAIN_CONF_H_

#include <string>
#include <iostream>
#include "libevent_http_frame.h"

using namespace std;

class CMainConf 
{
public:
    static bool InitConf(string &strServerName, string &strConfPath)
    {
        strServerName = "example";
        strConfPath = "./config.xml";

        return true;
    }
    static CFrame *GetFrame()
    {
        return new CLibeventHttpFrame();
    }
};

#endif
