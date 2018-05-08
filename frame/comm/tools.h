#ifndef _TOOLS_H_
#define _TOOLS_H_ 

#include <boost/asio.hpp>  

using boost::asio::ip::tcp;  
using namespace std;


namespace tools 
{

string GetHostNameIp() 
{  
    boost::asio::io_service io_service;  
    tcp::resolver resolver(io_service);  
    tcp::resolver::query query(boost::asio::ip::host_name(), "");  
    tcp::resolver::iterator iter = resolver.resolve(query);  
    tcp::resolver::iterator end;
    if(iter != end)
    {
        tcp::endpoint ep = *iter;  
        return ep.address().to_string();
    }
    return string("");
}  

}

#endif
