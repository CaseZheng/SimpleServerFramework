#include <event2/event.h>
#include <event2/buffer.h>
#include "log.h"
#include "pb_protocol.h"
#include "pb_packet_model.h"
#include "tools.h"

#ifdef EVBUFFER
bool CPbProtocol::Unpacking(boost::shared_ptr<struct evbuffer> &buff, IPacketModel *packet)
{
    DEBUG("=========== Unpacking Start ===========");
    if(NULL == buff.get())
    {
        ERROR("buff is NULL");
        return false;
    }
    int buffLen = evbuffer_get_length(buff.get());
    if(0 == buffLen)
    {
        return false;
    }
    CPbInPacketModel *pPacketModel = dynamic_cast<CPbInPacketModel*>(packet);
    struct evbuffer_ptr evbufferPtr = evbuffer_search(buff.get(), "PB", 2, NULL);
    if(evbufferPtr.pos < 0) //未找到PB标记
    {
        evbuffer_drain(buff.get(), buffLen);    //清空buff
        return false;
    }
    evbuffer_drain(buff.get(), evbufferPtr.pos);        //将PB标记前的数据删除掉
    evbufferPtr = evbuffer_search(buff.get(), "PB", 2, NULL);
    buffLen = evbuffer_get_length(buff.get());
    if(evbufferPtr.pos < 0) //未找到PB标记
    {
        evbuffer_drain(buff.get(), buffLen);    //清空buff
        return false;
    }
    if(buffLen < 6) //数据长度不足
    {
        return false;
    }
    char lenBuf[6];
    evbuffer_copyout_from(buff.get(), &evbufferPtr, (void*)&lenBuf, 6);
    int len = *((int*)&lenBuf[2]);
    DEBUG(to_string(len));
    if(len+6 < buffLen)
    {
        DEBUG("包未接收完毕,等待");
        return false;
    }

    len += 6;
    boost::shared_ptr<char> buf(new char[len]);
    evbuffer_remove(buff.get(), (void*)buf.get(), len);
    string strPacket(buf.get()+6, buf.get()+len);
    if(!pPacketModel->ParseFromString(strPacket))
    {
        DEBUG("解析报文出错");
    }

    DEBUG("=========== Unpacking End ===========");
    return true;
}
bool CPbProtocol::Packets(boost::shared_ptr<struct evbuffer> &buff, IPacketModel *packet)
{
    DEBUG("=========== Packets Start ===========");
    CPbOutPacketModel *pOutPacketModel = dynamic_cast<CPbOutPacketModel*>(packet);
    string strPacket;
    if(!pOutPacketModel->SerializeToString(&strPacket))
    {
        ERROR("打包失败");
        return false;
    }
    char lenBuff[6] = {0};
    lenBuff[0] = 'P';
    lenBuff[1] = 'B';
    int len = strPacket.size();
    memcpy(&lenBuff[2], (void*)&len, 4);
    evbuffer_add(buff.get(), (void*)lenBuff, sizeof(lenBuff));
    evbuffer_add(buff.get(), (void*)strPacket.c_str(), strPacket.size());

    DEBUG("=========== Packets End ===========");
    return true;
}

#else
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
        string strPacket(buff.begin()+6, buff.begin()+6+len);
        if(!pPacketModel->ParseFromString(strPacket))
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
#endif
