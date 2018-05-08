#include "main_conf.h"
#include "frame.h"

using namespace std;

int main()
{
    string strServerName;
    string strConfPath;
    if(!CMainConf::InitConf(strServerName, strConfPath))
    {
        std::cerr << "Function failed, CMainConf::InitConf" << std::endl;
        return -1;
    }

    CFrame oFrame;
    if(!oFrame.Init(strServerName, strConfPath))
    {
        std::cerr << "Function failed, oFrame.Init" << std::endl;
        return -1;
    }

    if(!oFrame.Run())
    {
        return -1;
    }

    return 0;
}
