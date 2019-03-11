#include "example_deal_model.h"
#include "example_packet_model.h"
#include "main_conf.h"

bool CExampleDealModel::DealPacket(boost::shared_ptr<CSocketHandle> &pSocketHandle, 
            boost::shared_ptr<IPacketModel> &pInPacketModel)
{
    CExampleInPacketModel *pExampleInPacketModel = dynamic_cast<CExampleInPacketModel*>(pInPacketModel.get());
    const string &msg = pExampleInPacketModel->GetMsg();
    DEBUG(msg);

    boost::shared_ptr<IPacketModel> pOutPacketModel(CMainConf::GetOutPacketModel());
    CExampleOutPacketModel *pExampleOutPacketModel = dynamic_cast<CExampleOutPacketModel*>(pOutPacketModel.get());
    pExampleOutPacketModel->SetMsg("hello client");
    pSocketHandle->WritePacket(pOutPacketModel);

    string strIp = "127.0.0.1";
    int iPort = 9992;

    int iSock;
    CLibeventTcpFrame::CreateClient(strIp, iPort, iSock);
    DEBUG(to_string(iSock));

    return true;
}
