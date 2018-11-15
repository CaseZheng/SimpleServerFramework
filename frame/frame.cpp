#include "frame.h"
#include "log.h"
#include "configure.h"

bool CFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(strServerName.empty())
    {
        m_strServerName = tools::GetProcessName();
    }
    else 
    {
        m_strServerName = strServerName;
    }
    if(strConfPath.empty())
    {
        m_strConfPath = "/data/config/config.xml";
    }
    else 
    {
        m_strConfPath = strConfPath;
    }

    //初始化配置文件
    CConfigure::InitConf(m_strConfPath);

    //初始化日志
    CLog::InitLog(m_strServerName);
    DEBUG("InitLog success");

    CConfigure::PrintConf();

    return true;
}

bool CFrame::Run()
{
    return true;
}

bool CFrame::Exit()
{
    DEBUG("Exit");
    exit(0);
    return true;
}
