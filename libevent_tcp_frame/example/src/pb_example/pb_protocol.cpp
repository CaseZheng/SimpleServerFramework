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

    DEBUG(tools::PrintHex(buff));

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
    for(auto &tmp : buff)
    {
        DEBUG(tmp);
    }

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
        DEBUG("buffsize:" + to_string(buff.size()));
        DEBUG(buff.front());

        DEBUG(tools::PrintHex(buff));

        if(buff.size() < 6+len)
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
    CPbOutPacketModel *pOutPacketModel = dynamic_cast<CPbOutPacketModel*>(packet);

    return true;
}
