#include "tools.h"

#include "log.h"

string tools::PrintHex(const vector<char>& buff)
{
    stringstream ss;
    for(auto i=0; i<buff.size(); ++i)
    {
        if(i%16 == 0)
        {
            ss << endl;
        }
        if(i%4 == 0)
        {
           ss << "0x";
        }
        ss << setfill('0') << setw(2) << hex << (int)buff[i];
        if(i%4 == 3)
        {
            ss << "  ";
        }
    }
    return ss.str();
}

