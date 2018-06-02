#ifndef _TOOLS_H_
#define _TOOLS_H_ 

#undef signal_set

#include <sstream>

#include <boost/asio.hpp>  
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/log/attributes/current_process_name.hpp>


using namespace std;

class tools 
{
public:
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

    static string GetRandomString()
    {
        using namespace boost::uuids;
        stringstream ss;
        random_generator rgen;
        uuid ranUUID = rgen();
        ss << ranUUID;
        return ss.str();
    }

    static string GetProcessName()
    {
        return boost::log::aux::get_process_name();
    }


    static string PrintHex(const vector<char>& buff);
};

#endif
