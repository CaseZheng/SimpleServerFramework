#include <sstream>
#include <boost/shared_ptr.hpp>

#include "public.h"
#include "tools.h"
#include "pb_packet_model.h"

#include "basic_protocol.pb.h"

template<class T>
void insert_number(T number, vector<char> &buf)
{
    cout << number << endl;
    cout << sizeof(T) << endl;
    char tmp[sizeof(T)] = {0};
    memcpy(tmp, (char*)&number, sizeof(T));
    for(int i=0; i<sizeof(T); ++i)
    {
        cout << "number:" << tmp[i] << endl;
        buf.push_back(tmp[i]);
    }
}

int main(int argc, char *argv[])
{
    io_service service;

    //socket对象
    ip::tcp::socket sock(service);

    //连接端点
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), PORT);
    
    boost::system::error_code error;
    //连接服务器
    sock.connect(ep, error);

    if(error)   //出错打印下出错信息
    {
        cout<<"connect error: "<<error<<endl;
        cout<<"connect error: "<<boost::system::system_error(error).what()<<endl;
        return -1;
    }

    boost::shared_ptr<CPbOutPacketModel> pPbOutPacket(new CPbOutPacketModel());  
    if(NULL == pPbOutPacket)
    {
        cout << "pPbOutPacket is NULL" <<endl;
        return -1;
    }
    pPbOutPacket->set_version(1);
    pPbOutPacket->set_time_stamps(1);
    pPbOutPacket->set_server_id(1);
    pPbOutPacket->set_invoked_id("faefafaefa");
    pPbOutPacket->set_sign_method(basic_protocol::PbBasicPackage::MD5);
    pPbOutPacket->set_signature("121");
    pPbOutPacket->set_boby("boby");

    string strPacket;
    pPbOutPacket->SerializeToString(&strPacket);

    std::vector<char> buf;
    buf.push_back('P');
    buf.push_back('B');
    cout << tools::PrintHex(buf) << endl;

    insert_number((int)(strPacket.size()), buf);
    cout << tools::PrintHex(buf) << endl;

    for(auto &it : strPacket)
    {
        buf.push_back(it);
    }
    cout << to_string(6+strPacket.size()-1) << endl;

    cout << tools::PrintHex(buf) << endl;

    sock.write_some(boost::asio::buffer(buf, buf.size()), error);
    if(error)
    {
        cout<<"write_some error:"<<error<<endl;
        cout<<"write_some error: "<<boost::system::system_error(error).what()<<endl;
    }
    sleep(3);
    return 0;
}
