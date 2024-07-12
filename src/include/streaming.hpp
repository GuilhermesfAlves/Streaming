#ifndef __STREAMING__
#define __STREAMING__
#include "utils.hpp"
#include "slidingWindow.hpp"
#include "stopNWait.hpp"

class Streaming{
protected:
    int action = T_INEXISTENT;
    int fileCount;
public:
    StopNWait single;
    SlidingWindow window;
    Streaming(string log_name);
    virtual ~Streaming() = default;

    void toConnect(char* connection);
    int virtual run() = 0;
};


#endif