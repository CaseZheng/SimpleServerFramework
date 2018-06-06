#include <event2/event.h>
#include <event2/buffer.h>
#include "example_protocol.h"
#include "example_packet_model.h"

#include "log.h"

#ifdef EVBUFFER
bool CExampleProtocol::Unpacking(boost::shared_ptr<struct evbuffer> &buff, IPacketModel *packet)
{
    if(NULL == buff.get())
    {
        ERROR("buff is NULL");
        return false;
    }
    int len = evbuffer_get_length(buff.get());
    if(0 == len)
    {
        return false;
    }
    CExampleInPacketModel *pPacketModel = dynamic_cast<CExampleInPacketModel*>(packet);
    boost::shared_ptr<char> buf(new char[len]);
    evbuffer_copyout(buff.get(), (void*)buf.get(), len);
    pPacketModel->SetMsg(string(buf.get()));
    evbuffer_drain(buff.get(), len);

    return true;
}
bool CExampleProtocol::Packets(boost::shared_ptr<struct evbuffer> &buff, IPacketModel *packet)
{
    DEBUG("=========== Packets Start ===========");
    CExampleOutPacketModel *pOutPacketModel = dynamic_cast<CExampleOutPacketModel*>(packet);
    const string &msg = pOutPacketModel->GetMsg();
    if(0 != evbuffer_add(buff.get(), msg.c_str(), msg.size()))
    {
        ERROR("evbuffer_add error");
        return false;
    }

    DEBUG("=========== Packets End ===========");

    return true;
}

#else

bool CExampleProtocol::Unpacking(vector<char> &buff, IPacketModel *packet)
{
    if(buff.empty())
    {
        return false;
    }
    CExampleInPacketModel *pPacketModel = dynamic_cast<CExampleInPacketModel*>(packet);
    pPacketModel->SetMsg(string(buff.begin(), buff.end()));
    buff.clear();

    return true;
}
bool CExampleProtocol::Packets(vector<char> &buff, IPacketModel *packet)
{
    DEBUG("=========== Packets Start ===========");
    CExampleOutPacketModel *pOutPacketModel = dynamic_cast<CExampleOutPacketModel*>(packet);
    const string &msg = pOutPacketModel->GetMsg();
    buff.insert(buff.begin(), msg.begin(), msg.end());

    DEBUG("=========== Packets End ===========");
    return true;
}
#endif
