#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_ 

#include <string>
#include <boost/shared_ptr.hpp>

#include "packet_model.h"

using namespace std;
using boost::shared_ptr;

class CHttpServer
{
public:
	CHttpServer(){}
	virtual ~CHttpServer(){}

	bool Init();
	bool Run();
    bool AddUri(const string &strUri, void (*cb)(evhttp_request *, void*), void *cbarg);

private:
	CHttpServer(const CHttpServer&);
	CHttpServer& operator=(const CHttpServer&);

    static void HttpCb(evhttp_request *pEvHttpRequest, void *pArg);
    static void HttpLog(int severity, const char *msg);

    boost::shared_ptr<struct event_base> m_pEventBase;
    boost::shared_ptr<struct evhttp> m_pEvHttp;
    boost::shared_ptr<CPacketModel> m_pPacketModel;

};

#endif
