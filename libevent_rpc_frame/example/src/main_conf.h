#ifndef _MAIN_CONF_H_
#define _MAIN_CONF_H_

#include <string>
#include <iostream>
#include "libevent_frame.h"
#include "libevent_rpc_frame.h"
#include "rpc_server.h"
#include "packet_model.h"
#include "deal_model.h"
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

        return true;
    }

    static CFrame *GetFrame()
    {
        return new CLibeventRpcFrame();
    }

    //添加uri和接口名及处理模块 自带协议
    static bool RegisterInterface(vector<InterfaceInfo> &vecInterfaceInfo)
    {
        vecInterfaceInfo.push_back(
                InterfaceInfo("test", 
                    CLogic::DealModel, 
                    static_cast<CLogic*>(new CExampleLogic()),
                    CMessageProcess::ReqNew,
                    NULL,
                    CMessageProcess::ReqFree,
                    CMessageProcess::ReqUnmarshal,
                    CMessageProcess::RplNew,
                    NULL,
                    CMessageProcess::RplFree,
                    CMessageProcess::RplComplete,
                    CMessageProcess::RplMarshal
                    ));
        return true;
    }
};

#endif
