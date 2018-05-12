#ifndef _PACKET_MODEL_H_
#define _PACKET_MODEL_H_

#include <string>
#include <map>
#include <evhttp.h>
#include <boost/shared_ptr.hpp>

#include "deal_model.h"


using namespace std;

class CPacketModel
{
public:
    CPacketModel(){}
    virtual ~CPacketModel(){}

    static bool AddInterface(const string& strUri, const string &strInterface, CDealModel *pDealModel);

    void InitInPacket(const enum evhttp_cmd_type eHttpType,
            const std::string& strIn,
            const std::string& strUrlPath, 
            const std::string& strUrlParams); 

    void DealPacket();

    string GetRespCode() { return m_strRespCode; }
    string GetOut()      { return m_strOut; }
    string GetReason()   { return m_strReason; }

private:
    static map<string, map<string, boost::shared_ptr<CDealModel>> > m_uriInterfaceDealModel;

    void Reset()
    {
        m_strRespCode = "200";
        m_strOut.clear(); 
        m_strReason.clear();
    }

    bool CheckAndUnpack();
    bool Deal();

    enum evhttp_cmd_type m_eHttpType;
    string m_strIn;
    string m_strUrlPath;
    string m_strUrlParams;

    string m_strRespCode;
    string m_strOut;
    string m_strReason;
};


#endif
