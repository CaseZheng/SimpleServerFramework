#ifndef _MAIN_CONF_H_
#define _MAIN_CONF_H_

#include <string>
#include <iostream>
#include "libevent_frame.h"
#include "log.h"
#include "tools.h"
#include "libevent_tcp_frame.h"
#include "packet_model.h"
#include "protocol.h"

#include "example_packet_model.h"
#include "example_deal_model.h"
#include "example_protocol.h"

using namespace std;

class CLibeventTcpFrame;

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
        return new(std::nothrow)CLibeventTcpFrame();
    }

    static IPacketModel* GetInPacketModel()
    {
        return new(std::nothrow)CExampleInPacketModel();
    }

    static IPacketModel* GetOutPacketModel()
    {
        return new(std::nothrow)CExampleOutPacketModel();
    }

    static IProtocol* GetProtocol()
    {
        return new(std::nothrow)CExampleProtocol();
    }

    static IDealModel* GetDealModel()
    {
        return new(std::nothrow)CExampleDealModel();
    }

};

#endif
