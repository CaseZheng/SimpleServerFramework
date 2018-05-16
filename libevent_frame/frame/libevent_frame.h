#ifndef _LIBEVENT_HTTP_FRAME_H_
#define _LIBEVENT_HTTP_FRAME_H_ 

#include <string>

#include "frame.h"

using namespace std;

class CLibeventFrame : public CFrame
{
public:
    CLibeventFrame(){}
    virtual ~CLibeventFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath);
    virtual bool Run();
};

#endif
