#ifndef _TOOLS_H_
#define _TOOLS_H_ 

#undef signal_set

#include <boost/asio.hpp>  
#include <boost/filesystem.hpp>

#include "log.h"

using namespace std;

namespace tools 
{

static string GetHostNameIp() 
{  
    using boost::asio::ip::tcp;  
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

static string GetFileNameFromFilePath(const string &strFilePath)
{
    using boost::filesystem::path;
    boost::filesystem::path boostPath(strFilePath);
    return boostPath.filename().generic_string();
}

}

#endif
