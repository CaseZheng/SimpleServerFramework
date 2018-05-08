#include "frame.h"
#include "log.h"
#include "configure.h"

bool CFrame::Init(const string &strServerName, const string &strConfPath)
{
    if(strServerName.empty() || strConfPath.empty())
    {
        return false;
    }

    m_strServerName = strServerName;
    m_strConfPath   = strConfPath;

    //初始化日志
    CLog::InitLog("./", m_strServerName);
    DEBUG("InitLog success");
    
    //初始化配置文件
    CConfigure::InitConf(strConfPath);
    CConfigure::PrintConf();

    return true;
}

bool CFrame::Run()
{
    return true;
}

