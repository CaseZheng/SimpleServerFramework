#ifndef _RPC_SERVER_H_
#define _RPC_SERVER_H_ 

#include <string>
#include <boost/shared_ptr.hpp>
#include "deal_model.h"

using namespace std;
using boost::shared_ptr;

typedef void(* CALL_BACK)(struct evrpc_req_generic *, void *);
typedef void*(* REQ_NEW)(void *);
typedef void(* REQ_FREE)(void *);
typedef int(* REQ_UNMARSHAL)(void*, struct evbuffer *);
typedef void*(* RPL_NEW)(void *);
typedef void(* RPL_FREE)(void *);
typedef int(* RPL_COMPLETE)(void *);
typedef void(* RPL_MARSHAL)(struct evbuffer *, void *);

typedef struct InterfaceInfo
{
    InterfaceInfo(const string& strName,
                CALL_BACK       pCallBack,
                CLogic*         pLogic,
                REQ_NEW         pReqNew,
                void *          pReqNewArg,
                REQ_FREE        pReqFree,
                REQ_UNMARSHAL   pReqUnmarshal,
                RPL_NEW         pRplNew,
                void *          pRplNewArg,
                RPL_FREE        pRplFree,
                RPL_COMPLETE    pRplComplete,
                RPL_MARSHAL     pRplMarshal)
        : m_pLogic(pLogic), m_strName(strName), m_pCallBack(pCallBack), 
            m_pReqNew(pReqNew), m_pReqNewArg(pReqNewArg), m_pReqFree(pReqFree), 
            m_pReqUnmarshal(pReqUnmarshal), m_pRplNew(pRplNew), m_pRplNewArg(pRplNewArg), 
            m_pRplFree(pRplFree), m_pRplComplete(pRplComplete), m_pRplMarshal(pRplMarshal)
    {
    }

    boost::shared_ptr<CLogic>  m_pLogic;        //
    string                     m_strName;       // rpc接口名称
    CALL_BACK                  m_pCallBack;     // 回调函数
    REQ_NEW                    m_pReqNew;       // 请求结构体创建函数
    void *                     m_pReqNewArg;    // 传入请求结构体创建函数的参数
    REQ_FREE                   m_pReqFree;      // 请求结构体释放函数
    REQ_UNMARSHAL              m_pReqUnmarshal; // 将缓存区数据转化为请求结构体的数据
    RPL_NEW                    m_pRplNew;       // 返回结构体创建函数
    void *                     m_pRplNewArg;    // 传入返回结构体创建函数的参数
    RPL_FREE                   m_pRplFree;      // 返回接头体释放函数
    RPL_COMPLETE               m_pRplComplete;  // 验证返回是否有效
    RPL_MARSHAL                m_pRplMarshal;   // 将返回结构体转化为缓存区信息
}InterfaceInfo;

class CRpcServer
{
public:
	CRpcServer(boost::shared_ptr<struct event_base> pEventBase) : m_pEventBase(pEventBase) {}
	virtual ~CRpcServer(){}

	bool Init();

private:
	CRpcServer(const CRpcServer&);
	CRpcServer& operator=(const CRpcServer&);

    boost::shared_ptr<struct event_base> m_pEventBase;
    boost::shared_ptr<struct evhttp> m_pEvHttp;
    boost::shared_ptr<struct evrpc_base> m_pEvRpcBase;

    vector<InterfaceInfo> m_vecInterfaceInfo;
};

#endif
