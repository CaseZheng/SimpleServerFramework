#include <event2/event.h>
#include <evhttp.h>
#include <event2/buffer.h>
#include <boost/shared_ptr.hpp>

#include "http_server.h"
#include "log.h"
#include "configure.h"
#include "main_conf.h"
#include "packet_model.h"

void CHttpServer::HttpLog(int severity, const char *msg)
{
    if(EVENT_LOG_DEBUG == severity || EVENT_LOG_MSG == severity)
    {
        DEBUG(msg);
    }
    else if(EVENT_LOG_WARN == severity)
    {
        WARNING(msg);
    }
    else
    {
        ERROR(msg);
    }
}

bool CHttpServer::Init()
{
    //event_enable_debug_logging(EVENT_DBG_ALL);
    event_enable_debug_logging(EVENT_DBG_NONE);
    event_set_log_callback(CHttpServer::HttpLog);

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

    m_pPacket.reset(new (std::nothrow) CPacketModel);
    if(NULL == m_pPacket)
    {
        ERROR("new CPacketModel failure");
        return false;
    }

    evhttp_set_gencb(m_pEvHttp.get(), &CPacketModel::HttpCb, m_pPacket.get());

    if(!CMainConf::AddInterface())
    {
        ERROR("CMainConf::AddInterface failure");
        return false;
    }

    if(!CMainConf::AddUri(m_vecUriInfo))
    {
        ERROR("CMainConf::AddUri failure");
        return false;
    }

    for(auto it=m_vecUriInfo.begin(); it!=m_vecUriInfo.end(); ++it)
    {
        if(!AddUri(it->m_strUri, CPacket::HttpCb, static_cast<void*>(it->m_cbarg.get())))
        {
            ERROR("添加Uri:" + it->m_strUri + "失败");
            return false;
        }
    }

    return true;
}

bool CHttpServer::AddUri(const string &strUri, EVHTTP_CB cb, void *cbarg)
{
    if(0 != evhttp_set_cb(m_pEvHttp.get(), strUri.c_str(), cb, cbarg))
    {
        ERROR("evhttp_set_cb failure");
        return false;
    }
    return true;
}

