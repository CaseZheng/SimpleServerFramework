#ifndef _EXAMPLE_PACKET_H_
#define _EXAMPLE_PACKET_H_

#include <evhttp.h>
#include <string>

#include "custom_packet_model.h"

using namespace std;

class CExamplePacket : public CPacket
{
public:
    CExamplePacket() {}
    virtual ~CExamplePacket() {}

    virtual bool DealPacket();

private:
    CExamplePacket(const CExamplePacket &);
    CExamplePacket & operator=(const CExamplePacket&);
};

#endif
