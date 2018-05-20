#include "log.h"
#include "example_timer.h"

bool CExampleTimer::TimerDeal()
{
    DEBUG("TimerDeal");
    return true;
}

bool CExampleTimer::GetTimeval(struct timeval &tv)
{
    memset(&tv, 0, sizeof(struct timeval));
    tv.tv_sec = 300;
    tv.tv_usec = 0;
    return true;
}
