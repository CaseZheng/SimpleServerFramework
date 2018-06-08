#include <event2/event.h>
#include <event2/buffer.h>
#include "example_protocol.h"
#include "example_packet_model.h"

#include "log.h"

bool CExampleProtocol::Unpacking(struct evbuffer *buff, IPacketModel *packet)
{
    if(NULL == buff)
    {
        ERROR("buff is NULL");
        return false;
    }
    int len = evbuffer_get_length(buff);
    if(0 == len)
    {
        return false;
    }
    CExampleInPacketModel *pPacketModel = dynamic_cast<CExampleInPacketModel*>(packet);
    boost::shared_ptr<char> buf(new char[len]);
    evbuffer_copyout(buff, (void*)buf.get(), len);
    pPacketModel->SetMsg(string(buf.get()));
    evbuffer_drain(buff, len);

    return true;
}
bool CExampleProtocol::Packets(struct evbuffer *buff, IPacketModel *packet)
{
    DEBUG("=========== Packets Start ===========");
    CExampleOutPacketModel *pOutPacketModel = dynamic_cast<CExampleOutPacketModel*>(packet);
    const string &msg = pOutPacketModel->GetMsg();
    if(0 != evbuffer_add(buff, msg.c_str(), msg.size()))
    {
        ERROR("evbuffer_add error");
        return false;
    }

    DEBUG("=========== Packets End ===========");

    return true;
}
