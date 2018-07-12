#include <event2/event.h>
#include <event2/rpc.h>
#include <evhttp.h>
#include <event2/buffer.h>
#include <boost/shared_ptr.hpp>

#include "rpc_server.h"
#include "log.h"
#include "configure.h"
#include "main_conf.h"
#include "packet_model.h"


bool CRpcServer::Init()
{
    m_pEvHttp.reset(evhttp_new(m_pEventBase.get()), evhttp_free);
    if(NULL == m_pEvHttp)
    {
        ERROR("evhttp_new failure");
        return false;
    }

    if(0 != evhttp_bind_socket(m_pEvHttp.get(), 
                CConfigure::GetIp().c_str(), 
                ::atoi(CConfigure::GetPort().c_str()) ))
    {
        ERROR("evhttp_bind_socket failure");
        return false;
    }

    m_pEvRpcBase.reset(evrpc_init(m_pEvHttp.get()), evrpc_free);
    if(NULL == m_pEvRpcBase)
    {
        ERROR("evrpc_init failure");
        return false;
    }

    if(!CMainConf::RegisterInterface(m_vecInterfaceInfo))
    {
        ERROR("CMainConf::RegisterInterface failure");
        return false;
    }

    for(auto it : m_vecInterfaceInfo)
    {
        DEBUG("interface: " + it.m_strName);
        evrpc_register_generic(m_pEvRpcBase.get(),     // rpc_base
                it.m_strName.c_str(),                  // rpc接口名称
                it.m_pCallBack,                        // 回调函数
                static_cast<void*>(it.m_pLogic.get()), // 回调函数参数
                it.m_pReqNew,                          // 请求结构体创建函数
                it.m_pReqNewArg,                       // 传入请求结构体创建函数的参数
                it.m_pReqFree,                         // 请求结构体释放函数
                it.m_pReqUnmarshal,                    // 将缓存区数据转化为请求结构体的数据
                it.m_pRplNew,                          // 返回结构体创建函数
                it.m_pRplNewArg,                       // 传入返回结构体创建函数的参数
                it.m_pRplFree,                         // 返回接头体释放函数
                it.m_pRplComplete,                     // 验证返回是否有效
                it.m_pRplMarshal);                     // 将返回结构体转化为缓存区信息
    }

    return true;
}
