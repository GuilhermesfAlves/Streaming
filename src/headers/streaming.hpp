#ifndef __STREAMING__
#define __STREAMING__
#include "utils.hpp"
#include "socket.hpp"

class Streaming{
public:
    MySocket *socket;
    Streaming(char* log_name): socket(new MySocket(log_name)){}
    virtual ~Streaming() = default;

    void virtual toConnect(int port) = 0;
    int virtual run() = 0;
};


#endif