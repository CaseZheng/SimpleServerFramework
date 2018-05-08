#ifndef _LOG_H_
#define _LOG_H_

#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/exceptions.hpp>

#include <iostream>
#include <boost/log/utility/setup/file.hpp>
//#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <iostream>  
#include <boost/shared_ptr.hpp>  
#include <boost/make_shared.hpp>  
#include <boost/filesystem.hpp>  

#include "tools.h"

namespace logging  = boost::log;
namespace sinks    = boost::log::sinks;
namespace attrs    = boost::log::attributes;
namespace src      = boost::log::sources;
namespace expr     = boost::log::expressions;
namespace keywords = boost::log::keywords;

using namespace std;
using boost::shared_ptr;

enum severity_level  
{  
    debug,
    info,
    warning,  
    error,  
    fatal, 
    report
};  

template< typename CharT, typename TraitsT >  
inline std::basic_ostream< CharT, TraitsT >& operator<< (  
            std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)  
{  
        static const char* const str[] =  
        {  
            "DEBUG",  
            "INFO",  
            "WARNING",  
            "ERROR",  
            "FATAL",  
            "REPORT"
        };  
        if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))  
            strm << str[lvl];
        else  
            strm << static_cast< int >(lvl);  
        return strm;  
}  

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::severity_logger_mt<severity_level>);

class CLog
{
public:
    static void SetFilter(severity_level filterLevel)
    {
        logging::core::get()->set_filter(expr::attr<severity_level>("Severity")>=filterLevel);
    }

    static bool InitLog(const string &strLogPath, const string &strLogName)
    {
        SetFilter(debug);

        string strHostNameIp = tools::GetHostNameIp();

        typedef sinks::synchronous_sink<sinks::text_file_backend> TextSink;
        boost::shared_ptr<sinks::text_file_backend> backend_file = boost::make_shared<sinks::text_file_backend>(  
            keywords::file_name = strLogPath + "/" + strLogName + "-%Y%m%d.%N.log",  
            keywords::rotation_size = 10 * 1024 * 1024,  
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),  
            keywords::min_free_space = 30 * 1024 * 1024
            );

        boost::shared_ptr<TextSink> sink_file(new TextSink(backend_file));  
        //时间|日志等级|进程id|线程id|本地ip|服务名|文件名|函数名|所在行|日志正文
        string strFormat = "%1%|%2%|%3%|%4%|"+strHostNameIp+"|%5%|%6%";
        sink_file->set_formatter (  
            expr::format("%1%|%2%|%3%|%4%|%5%|"+strHostNameIp+"|%6%")  
            % expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y%m%d-%H%M%S.%f")  
            % expr::attr<severity_level>("Severity")  
            % expr::attr<attrs::current_process_id::value_type>("ProcessID")  
            % expr::attr<attrs::current_thread_id::value_type >("ThreadID")  
            % expr::attr<attrs::current_process_name::value_type >("ProcessName")  
            % expr::smessage  
            );  

        logging::core::get()->add_sink(sink_file);
        sink_file->set_filter(expr::attr<severity_level>("Severity") >= debug);

        boost::shared_ptr<sinks::text_file_backend> backend_report = boost::make_shared<sinks::text_file_backend>(  
            keywords::file_name = strLogPath + "/" + strLogName + "-%Y%m%d.report.log",  
            keywords::rotation_size = 100 * 1024 * 1024,  
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(boost::gregorian::greg_day(1), 0, 0, 0),  
            keywords::min_free_space = 30 * 1024 * 1024
            );

        backend_report->auto_flush(true);  
        boost::shared_ptr<TextSink> sink_report(new TextSink(backend_report));  
        sink_report->set_formatter (  
            expr::format("%1%,%2%")  
            % expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")  
            % expr::smessage 
            );  

        sink_report->set_filter(expr::attr<severity_level>("Severity") == report);  
        logging::core::get()->add_sink(sink_report);  

        logging::add_common_attributes();  
        logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
        logging::core::get()->add_global_attribute("ProcessID", attrs::current_process_id());
        logging::core::get()->add_global_attribute("ProcessName", attrs::current_process_name());
 
        return true;
    }

    static void log_print(const severity_level &level, const string &msg)
    {
        BOOST_LOG_SEV(my_logger::get(), level) << msg;
    }
};

#define LOG_PRINT(level, msg) CLog::log_print((level), (string(__FILE__)+"|"+__FUNCTION__+"|"+to_string(__LINE__)+"|"+(msg)))

#define INFO(msg)       LOG_PRINT(info, (msg))
#define DEBUG(msg)      LOG_PRINT(debug, (msg))
#define WARNING(msg)    LOG_PRINT(warning, (msg))
#define ERROR(msg)      LOG_PRINT(error, (msg))
#define FATAL(msg)      LOG_PRINT(fatal, (msg))
#define REPORT(msg)     CLog::log_print(report, (msg))

#endif
