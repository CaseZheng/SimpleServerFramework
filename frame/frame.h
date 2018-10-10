#ifndef _FRAME_H_
#define _FRAME_H_ 
#include <string>

using namespace std;

class IFrame 
{
public:
    IFrame() : m_strServerName{""}, m_strConfPath{""} {}
    virtual ~IFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath)=0;
    virtual bool Run()=0;
    virtual bool Exit()=0;
private:
    IFrame(const IFrame&);
    IFrame& operator=(const IFrame&);

protected:
    string m_strServerName;
    string m_strConfPath;
};


class CFrame : public IFrame 
{
public:
    CFrame(){}
    virtual ~CFrame(){}

    virtual bool Init(const string &strServerName, const string &strConfPath);
    virtual bool Run();
    virtual bool Exit();
};

#endif
