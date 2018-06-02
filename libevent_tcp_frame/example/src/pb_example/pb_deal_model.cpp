#include <sstream>
#include "pb_deal_model.h"
#include "pb_packet_model.h"
#include "main_conf.h"

bool CPbDealModel::DealPacket(boost::shared_ptr<CSocketHandle> &pSocketHandle, 
            boost::shared_ptr<IPacketModel> &pInPacketModel)
{
    DEBUG("=========== Deal Start ============");
    CPbInPacketModel *pPbInPacketModel = dynamic_cast<CPbInPacketModel*>(pInPacketModel.get());
    PrintPackageInfo(pPbInPacketModel);

    //boost::shared_ptr<IPacketModel> pOutPacketModel(CMainConf::GetOutPacketModel());
    //CPbOutPacketModel *pPbOutPacketModel = dynamic_cast<CPbOutPacketModel*>(pOutPacketModel.get());
    //pSocketHandle->WritePacket(pOutPacketModel);

    DEBUG("=========== Deal End ============");
    return true;
}

void CPbDealModel::PrintPackageInfo(CPbInPacketModel *pPbInPacketModel)
{
    stringstream ss;
    ss << "version:"     << pPbInPacketModel->version()     << "  ";
    ss << "time_stamps:" << pPbInPacketModel->time_stamps() << "  ";
    ss << "server_id:"   << pPbInPacketModel->server_id()   << "  ";
    ss << "invoked_id:"  << pPbInPacketModel->invoked_id()  << "  ";
    ss << "sign_method:" << pPbInPacketModel->sign_method() << "  ";
    ss << "signature:"   << pPbInPacketModel->signature()   << "  ";
    DEBUG(ss.str());
}
