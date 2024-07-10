#ifndef __STREAMING__
#define __STREAMING__
#include "utils.hpp"
#include "slidingWindow.hpp"

class Streaming{
public:
    SlidingWindow window;
    Streaming(string log_name);
    virtual ~Streaming() = default;

    void toConnect(char* connection);
    int virtual run() = 0;
};


#endif