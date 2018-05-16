#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_ 

#include <string>
#include <boost/shared_ptr.hpp>

#include "custom_packet_model.h"

using namespace std;
using boost::shared_ptr;

typedef void(* EVHTTP_CB)(evhttp_request *, void *);

typedef struct UriInfo
{
    UriInfo(const string& strUri, CPacket* cbarg) 
        : m_strUri(strUri), m_cbarg(cbarg) {}

    string m_strUri;
    boost::shared_ptr<CPacket> m_cbarg;
} UriInfo;

class CHttpServer
{
public:
	CHttpServer(boost::shared_ptr<struct event_base> pEventBase) : m_pEventBase(pEventBase) {}
	virtual ~CHttpServer(){}

	bool Init();
    bool AddUri(const string &strUri, EVHTTP_CB cb, void *cbarg);

private:
	CHttpServer(const CHttpServer&);
	CHttpServer& operator=(const CHttpServer&);

    static void HttpLog(int severity, const char *msg);

    boost::shared_ptr<struct event_base> m_pEventBase;
    boost::shared_ptr<struct evhttp> m_pEvHttp;
    boost::shared_ptr<CPacket> m_pPacket;
    vector<UriInfo> m_vecUriInfo;
};

#endif
