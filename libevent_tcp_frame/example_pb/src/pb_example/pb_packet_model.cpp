#include "pb_packet_model.h"

#include <sstream>

#include "log.h"
#include "tools.h"
#include "configure.h"

CPbInPacketModel::CPbInPacketModel()
{
    set_version(1);
    set_time_stamps(tools::GetTimeStamps());
    set_server_id(CConfigure::GetServiceId());
    set_invoked_id(tools::GetRandomString());
}

CPbOutPacketModel::CPbOutPacketModel()
{
    set_version(1);
    set_time_stamps(tools::GetTimeStamps());
    set_server_id(CConfigure::GetServiceId());
    set_invoked_id(tools::GetRandomString());
}
