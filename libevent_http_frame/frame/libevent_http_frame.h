#ifndef _LIBEVENT_HTTP_FRAME_H_
#define _LIBEVENT_HTTP_FRAME_H_ 

#include <string>

#include "libevent_frame.h"
#include "http_server.h"

using namespace std;

class CLibeventHttpFrame : public CLibeventFrame
{
public:
    CLibeventHttpFrame(){}
    virtual ~CLibeventHttpFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath);

private:

    boost::shared_ptr<CHttpServer> m_pHttpServer;
};

#endif
