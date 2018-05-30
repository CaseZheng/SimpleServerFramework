#ifndef _PB_PACKET_MODEL_H_
#define _PB_PACKET_MODEL_H_

#include "packet_model.h"
#include <vector>

using namespace std;

class CPbInPacketModel : public IPacketModel 
{
public:
    CPbInPacketModel() {}
    virtual ~CPbInPacketModel() {}

    void SetMsg(const string &strMsg) { m_strMsg = strMsg; }
    const string &GetMsg() { return m_strMsg; }
private:
    std::string m_strMsg;
};

class CPbOutPacketModel : public IPacketModel 
{
public:
    CPbOutPacketModel() {}
    virtual ~CPbOutPacketModel() {}

    void SetMsg(const string &strMsg) { m_strMsg = strMsg; }
    const string &GetMsg() { return m_strMsg; }
private:
    std::string m_strMsg;
};


#endif
