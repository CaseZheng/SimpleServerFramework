#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_ 

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

class CTcpClient : public boost::noncopyable, public boost::enable_shared_from_this<CTcpClient>, public CSocketManage
{
public:
	CTcpClient(boost::shared_ptr<struct event_base> pEventBase) : m_pEventBase(pEventBase) {}
	virtual ~CTcpClient(){}

	bool Init();
    void EraseSocketHandleBySocket(int sock);

    const boost::shared_ptr<IProtocol> &GetIProtocol() { return m_pProtocol; }
    const boost::shared_ptr<IDealModel> &GetIDealModel() { return m_pDealModel; }

private:
    bool CreateConnection(const string& strHost, int iPort, int &iSock);
    static void SocketReadCb(int fd, short events, void *arg);
    static void SocketWriteCb(int fd, short events, void *arg);

    struct event_base* GetEventBase() { return m_pEventBase.get(); }
private:
    boost::shared_ptr<struct evconnlistener> m_pEvConnListener;
    boost::shared_ptr<struct event_base> m_pEventBase;

    boost::shared_ptr<IProtocol> m_pProtocol;
    boost::shared_ptr<IDealModel> m_pDealModel;
};

#endif
