#include <sstream>
#include <boost/shared_ptr.hpp>

#include "public.h"
#include "tools.h"
#include "pb_client_packet_model.h"

#include "basic_protocol.pb.h"

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

    boost::shared_ptr<CPbClientOutPacketModel> pPbClientOutPacket(new CPbClientOutPacketModel());  
    if(NULL == pPbClientOutPacket)
    {
        cout << "pPbClientOutPacket is NULL" <<endl;
        return -1;
    }
    pPbClientOutPacket->set_sign_method(basic_protocol::PbBasicPackage::MD5);
    pPbClientOutPacket->set_signature("signature");
    pPbClientOutPacket->set_boby("hello server");

    string strPacket;
    pPbClientOutPacket->SerializeToString(&strPacket);

    std::vector<char> buf;
    buf.push_back('P');
    buf.push_back('B');
    cout << tools::PrintHex(buf) << endl;

    tools::insert_number((int)(strPacket.size()), buf);
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

    sleep(2);

	buf.clear();
    buf.resize(1000);
    size_t len = sock.read_some(boost::asio::buffer(buf), error);
    if(error)
    {
        cout<<"error eof: "<<error<<endl;
        cout<<"error eof: "<<boost::system::system_error(error).what()<<endl;
    }

    if(buf.size() < 6)
    {
        cout<<"buf.size() is less 6"<<endl;
        return -1;
    }
    cout<<buf[0]<<buf[1]<<endl;
    len = *((int*)&buf[2]);
    cout<<len<<endl;
    if(buf.size() < (unsigned)(6+len))
    {
        cout<<"buf.size() is less 6+len"<<endl;
        return -1;
    }
    string packet(buf.begin()+6, buf.begin()+6+len);
    boost::shared_ptr<CPbClientInPacketModel> pPbClientInPacket(new CPbClientInPacketModel());  
    if(!pPbClientInPacket->ParseFromString(packet))
    {
        cout<<"解析报文出错"<<endl;
        return -1;
    }
    stringstream ss;
    ss << "version:"     << pPbClientInPacket->version()     << "  ";
    ss << "time_stamps:" << pPbClientInPacket->time_stamps() << "  ";
    ss << "server_id:"   << pPbClientInPacket->server_id()   << "  ";
    ss << "invoked_id:"  << pPbClientInPacket->invoked_id()  << "  ";
    ss << "sign_method:" << pPbClientInPacket->sign_method() << "  ";
    ss << "signature:"   << pPbClientInPacket->signature()   << "  ";
    cout<<(ss.str())<<endl;

    sleep(2);
    return 0;
}
