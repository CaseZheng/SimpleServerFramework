#include "pb_deal_model.h"
#include "pb_packet_model.h"
#include "main_conf.h"

bool CPbDealModel::DealPacket(boost::shared_ptr<CSocketHandle> &pSocketHandle, 
            boost::shared_ptr<IPacketModel> &pInPacketModel)
{
    CPbInPacketModel *pPbInPacketModel = dynamic_cast<CPbInPacketModel*>(pInPacketModel.get());
    const string &msg = pPbInPacketModel->GetMsg();
    DEBUG(msg);

    boost::shared_ptr<IPacketModel> pOutPacketModel(CMainConf::GetOutPacketModel());
    CPbOutPacketModel *pPbOutPacketModel = dynamic_cast<CPbOutPacketModel*>(pOutPacketModel.get());
    pPbOutPacketModel->SetMsg("hello client");
    pSocketHandle->WritePacket(pOutPacketModel);

    return true;
}
