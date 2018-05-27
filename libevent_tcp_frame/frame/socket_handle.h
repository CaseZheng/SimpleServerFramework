#ifndef _SOCKET_HANDLE_H_
#define _SOCKET_HANDLE_H_

#include <string>
#include <sys/socket.h>
#include <boost/shared_ptr.hpp>
#include "packet_model.h"
#include <event2/event.h>
#include "log.h"

using namespace std;

class CSocketHandle
{
public:
    CSocketHandle(int sock, const string &strClientIp);

    virtual ~CSocketHandle()
    {
        ErrorOrCloseFd();
    }

    void SetReadEvent(boost::shared_ptr<struct event> &pEvent);
    void SetWriteEvent(boost::shared_ptr<struct event> &pEvent);
    void ErrorOrCloseFd();
    void WriteBuffer(char *buffer, int len);
    void ConsumeData();
    //void ReadBuffer();

private:
    int m_iSocketFd;
    string m_strClientIp;
    boost::shared_ptr<struct event> m_pReadEvent;
    boost::shared_ptr<struct event> m_pWriteEvent;
    vector<char> m_vReadBuffer;
    vector<char> m_vWriterBuffer;

    boost::shared_ptr<IPacketModel> m_pPacketModel;
};

#endif

