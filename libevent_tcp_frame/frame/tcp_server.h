#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_ 

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/listener.h>

#include "log.h"
#include "timer.h"
#include "socket_handle.h"

using namespace std;
using boost::shared_ptr;


typedef evconnlistener_cb EVCONNLISTENER_CB;

class CSocketHandle;

class CTcpServer : public boost::noncopyable, public boost::enable_shared_from_this<CTcpServer>
{
public:
	CTcpServer(boost::shared_ptr<struct event_base> pEventBase) : m_pEventBase(pEventBase) {}
	virtual ~CTcpServer(){}

	bool Init();
    void EraseSocketHandleBySocket(int sock);

private:
    static void EvConnListenerCb(struct evconnlistener * stener, 
            evutil_socket_t sock, 
            struct sockaddr* addr,
            int len,
            void * ptr);

    static void SocketReadCb(int fd, short events, void *arg);
    static void SocketWriteCb(int fd, short events, void *arg);

    struct event_base* GetEventBase() { return m_pEventBase.get(); }
    boost::shared_ptr<CSocketHandle> GetSocketHandleByFd(int fd);
    void InsertSocketHandle(int sock, boost::shared_ptr<CSocketHandle> &pSocketHandle);

private:
    boost::shared_ptr<struct evconnlistener> m_pEvConnListener;
    boost::shared_ptr<struct event_base> m_pEventBase;
    std::map<int, boost::shared_ptr<CSocketHandle>> m_mSocketHandle;
};

#endif
