#include "example_packet.h"
#include <evhttp.h>
#include "log.h"

bool CExamplePacket::DealPacket()
{
    m_strRespCode = "200";
    m_strOut      = "hello ExamplePacket";
    m_strReason   = "";
    return true;
}
