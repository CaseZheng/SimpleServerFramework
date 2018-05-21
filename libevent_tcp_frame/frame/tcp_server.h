#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_ 

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/listener.h>

#include "log.h"
#include "timer.h"

using namespace std;
using boost::shared_ptr;


typedef evconnlistener_cb EVCONNLISTENER_CB;

class CTcpServer : public boost::noncopyable
{
public:
	CTcpServer(boost::shared_ptr<struct event_base> pEventBase) : m_pEventBase(pEventBase) {}
	virtual ~CTcpServer(){}

	bool Init();

private:
    static void EvConnListenerCb(struct evconnlistener * stener, 
            evutil_socket_t sock, 
            struct sockaddr* addr,
            int len,
            void * ptr);

private:
    boost::shared_ptr<struct evconnlistener> m_pEvConnListener;
    boost::shared_ptr<struct event_base> m_pEventBase;
};

#endif
