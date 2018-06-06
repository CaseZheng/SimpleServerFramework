#include "pb_client_packet_model.h"

#include <sstream>

#include "log.h"
#include "tools.h"

CPbClientInPacketModel::CPbClientInPacketModel()
{
    set_version(1);
    set_time_stamps(tools::GetTimeStamps());
    set_server_id("1");
    set_invoked_id(tools::GetRandomString());
}

CPbClientOutPacketModel::CPbClientOutPacketModel()
{
    set_version(1);
    set_time_stamps(tools::GetTimeStamps());
    set_server_id("1");
    set_invoked_id(tools::GetRandomString());
}
