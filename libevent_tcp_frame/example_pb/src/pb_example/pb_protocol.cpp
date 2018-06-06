#include "log.h"
#include "pb_protocol.h"
#include "pb_packet_model.h"
#include "tools.h"

bool CPbProtocol::Unpacking(vector<char> &buff, IPacketModel *packet)
{
    if(buff.empty())
    {
        DEBUG("buff is empty");
        return false;
    }

    auto it = buff.begin();
    for(;it!=buff.end(); ++it)
    {
        if('P' == *it)
        {
            break;
        }
    }
    if(it == buff.end())
    {
        DEBUG("can't find \"P\"");
        buff.clear();
        return false;
    }

    buff.erase(buff.begin(), it);
    it = buff.begin();
    if('B' == *(++it))
    {
        CPbInPacketModel *pPacketModel = dynamic_cast<CPbInPacketModel*>(packet);
        if(buff.size() < 6)
        {
            DEBUG("包头部尺寸不足");
            return false;
        }
        int len = *((int*)&buff[2]);
        DEBUG("length:" + to_string(len));
        DEBUG(tools::PrintHex(buff));

        if(buff.size() < (unsigned)(6+len))
        {
            DEBUG("包未接收完毕,等待");
            return false;
        }
        string packet(buff.begin()+6, buff.begin()+6+len);
        if(!pPacketModel->ParseFromString(packet))
        {
            DEBUG("解析报文出错");
        }
        buff.erase(buff.begin(), (buff.begin()+6+len));
    }
    else 
    {
        DEBUG("can't find \"B\"");
        buff.erase(buff.begin());
        return false;
    }
    return true;
}

bool CPbProtocol::Packets(vector<char> &buff, IPacketModel *packet)
{
    DEBUG("=========== Packets Start ===========");
    CPbOutPacketModel *pOutPacketModel = dynamic_cast<CPbOutPacketModel*>(packet);
    string strPacket;
    if(!pOutPacketModel->SerializeToString(&strPacket))
    {
        ERROR("打包失败");
        return false;
    }
    buff.push_back('P');
    buff.push_back('B');
    tools::insert_number((int)(strPacket.size()), buff);
    buff.insert(buff.end(), strPacket.c_str(), strPacket.c_str()+strPacket.size());

    DEBUG(tools::PrintHex(buff));

    DEBUG("=========== Packets End ===========");
    return true;
}
