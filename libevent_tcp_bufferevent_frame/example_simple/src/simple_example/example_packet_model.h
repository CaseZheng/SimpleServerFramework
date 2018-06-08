#ifndef _EXAMPLE_PACKET_MODEL_H_
#define _EXAMPLE_PACKET_MODEL_H_

#include "packet_model.h"
#include <vector>

using namespace std;

class CExampleInPacketModel : public IPacketModel 
{
public:
    CExampleInPacketModel() {}
    virtual ~CExampleInPacketModel() {}

    void SetMsg(const string &strMsg) { m_strMsg = strMsg; }
    const string &GetMsg() { return m_strMsg; }
private:
    std::string m_strMsg;
};

class CExampleOutPacketModel : public IPacketModel 
{
public:
    CExampleOutPacketModel() {}
    virtual ~CExampleOutPacketModel() {}

    void SetMsg(const string &strMsg) { m_strMsg = strMsg; }
    const string &GetMsg() { return m_strMsg; }
private:
    std::string m_strMsg;
};


#endif
