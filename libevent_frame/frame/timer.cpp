#include "timer.h"
#include "log.h"

CTimer::CTimer(boost::shared_ptr<struct event_base> pEventBase, EVTIMER_CB cb, void* arg)
    : m_cb(cb), m_arg(arg), m_pEventBase(pEventBase)
{
    m_pEvent.reset(evtimer_new(m_pEventBase.get(), m_cb, m_arg), event_free);
}

bool CTimer::AddTimer(const struct timeval &tv)
{
    if(NULL == m_pEventBase)
    {
        DEBUG("m_pEventBase is NULL");
        return false;
    }
    if(NULL == m_pEvent)
    {
        DEBUG("m_pEvent is NULL");
        return false;
    }
    if(0 != evtimer_add(m_pEvent.get(), &tv))
    {
        DEBUG("evtimer_add failure");
        return false;
    }
    DEBUG("evtimer_add success");
    return true;
}

bool CTimer::DelTimer()
{
    if(NULL == m_pEvent)
    {
        DEBUG("m_pEvent is NULL");
        return false;
    }
    if(0 != evtimer_del(m_pEvent.get()))
    {
        DEBUG("evtimer_del failure");
        return false;
    }
    DEBUG("evtimer_del success");
    return true;
}

