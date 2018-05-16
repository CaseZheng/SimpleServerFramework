#ifndef _CUSTOM_PACKET_MODEL_H_
#define _CUSTOM_PACKET_MODEL_H_

#include <evhttp.h>
#include <string>

using namespace std;

class CPacket 
{
public:
    CPacket() {}
    virtual ~CPacket() {}

    static void HttpCb(evhttp_request *, void*);

    virtual bool DealPacket()=0;

    bool HttpCbStart();
    bool HttpCbEnd();

private:
    CPacket(const CPacket &);
    CPacket & operator=(const CPacket&);
    void Reset()
    {
        m_strRespCode = "200";
        m_strOut.clear(); 
        m_strReason.clear();
        m_pEvHttpRequest = NULL;
    }


protected:
    enum evhttp_cmd_type m_eHttpType;
    string m_strIn;
    string m_strUrlPath;
    string m_strUrlParams;

    string m_strRespCode;
    string m_strOut;
    string m_strReason;

    evhttp_request *m_pEvHttpRequest;
};


#endif
