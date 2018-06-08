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
#include "protocol.h"
#include "deal_model.h"

using namespace std;
using boost::shared_ptr;

class IDealModel;


typedef evconnlistener_cb EVCONNLISTENER_CB;

class CSocketHandle;

class CTcpServer : public boost::noncopyable, public boost::enable_shared_from_this<CTcpServer>
{
public:
	CTcpServer(boost::shared_ptr<struct event_base> pEventBase) : m_pEventBase(pEventBase) {}
	virtual ~CTcpServer(){}

	bool Init();
    void EraseSocketHandleBySocket(int sock);

    const boost::shared_ptr<IProtocol> &GetIProtocol() { return m_pProtocol; }
    const boost::shared_ptr<IDealModel> &GetIDealModel() { return m_pDealModel; }

private:
    static void EvConnListenerCb(struct evconnlistener * stener, 
            evutil_socket_t sock, 
            struct sockaddr* addr,
            int len,
            void * ptr);

    static void SocketReadCb(struct bufferevent *bev, void *ctx);
    static void SocketWriteCb(struct bufferevent *bev, void *ctx);
    static void SocketEventCb(struct bufferevent *bev, short what, void *ctx);

    struct event_base* GetEventBase() { return m_pEventBase.get(); }
    boost::shared_ptr<CSocketHandle> GetSocketHandleByFd(int fd);
    void InsertSocketHandle(int sock, boost::shared_ptr<CSocketHandle> &pSocketHandle);

private:
    boost::shared_ptr<struct evconnlistener> m_pEvConnListener;
    boost::shared_ptr<struct event_base> m_pEventBase;
    std::map<int, boost::shared_ptr<CSocketHandle>> m_mSocketHandle;

    boost::shared_ptr<IProtocol> m_pProtocol;
    boost::shared_ptr<IDealModel> m_pDealModel;
};

#endif
