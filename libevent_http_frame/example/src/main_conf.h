#ifndef _MAIN_CONF_H_
#define _MAIN_CONF_H_

#include <string>
#include <iostream>
#include "libevent_frame.h"
#include "libevent_http_frame.h"
#include "http_server.h"
#include "packet_model.h"
#include "log.h"

#include "example_logic.h"
#include "example_packet.h"

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
        return new CLibeventHttpFrame();
    }

    //添加uri 和 处理该uri的处理模块 无协议 需自己处理
    static bool AddUri(vector<UriInfo> &vecUriInfo)
    {
        vecUriInfo.push_back( UriInfo(string("/example/uri"), new (std::nothrow)CExamplePacket) );
        return true;
    }

    //添加uri和接口名及处理模块 自带协议
    static bool AddInterface()
    {
        if(!CPacketModel::AddInterface("/example/interface", "test", new (std::nothrow) CExampleLogic))
        {
            ERROR("AddInterface failure");
            return false;
        }
        return true;
    }
};

#endif
