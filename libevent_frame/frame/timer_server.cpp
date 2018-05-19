#include "main_conf.h"
#include "timer_server.h"

ITimerServer::ITimerServer()
{
}

void ITimerServer::TimerCb(evutil_socket_t fd, short events, void * arg)
{
    ITimerServer * pTimerServer = static_cast<ITimerServer*>(arg);
    if(NULL == pTimerServer)
    {
        return;
    }
    if(!pTimerServer->TimerCbStart())
    {
        ERROR("TimerCbStart failure");
        return ;
    }
    pTimerServer->TimerDeal();
    pTimerServer->TimerCbEnd();
}

bool ITimerServer::TimerCbStart()
{
    DEBUG("ITimerServer Start");

    if(!m_pTimer->DelTimer())
    {
        ERROR("DelTimer failure");
        return false;
    }
    return true;
}

bool ITimerServer::TimerCbEnd()
{
    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    if(!GetTimeval(tv))
    {
        ERROR("GetTimerval failure");
        return false;
    }
    if(!m_pTimer->AddTimer(tv))
    {
        ERROR("AddTimer failure");
        return false;
    }
    DEBUG("ITimerServer End");
    return true;
}

bool ITimerServer::StartTimerServer()
{
    if(NULL == m_pTimer)
    {
        ERROR("m_pTimer is NULL");
        return false;
    }
    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    if(!GetTimeval(tv))
    {
        ERROR("GetTimerval failure");
        return false;
    }
    if(!m_pTimer->AddTimer(tv))
    {
        ERROR("AddTimer failure");
        return false;
    }
    return true;
}

vector<boost::shared_ptr<ITimerServer>> CTimerServer::m_pTimerServer;

bool CTimerServer::Init()
{
    if(!CMainConf::InsertTimer())
    {
        ERROR("CMain::InsertTimer failure");
        return false;
    }

    for(auto &it : m_pTimerServer)
    {
        if(NULL == it)
        {
            ERROR("it is NULL");
            return false;
        }
        it->ResetTimer(
                new (std::nothrow)CTimer(m_pEventBase, 
                                    ITimerServer::TimerCb, 
                                    static_cast<void*>(it.get()))
                );
        if(!it->StartTimerServer())
        {
            ERROR("StartTimerServer failure");
            return false;
        }
    }
    return true;
}

bool CTimerServer::InsertTimer(ITimerServer *pTimerServer)
{
    m_pTimerServer.push_back(boost::shared_ptr<ITimerServer>(pTimerServer));
    return true;
}

