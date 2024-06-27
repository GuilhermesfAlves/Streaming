#ifndef __STREAMING__
#define __STREAMING__
#include "utils.hpp"

class Streaming{
public:
    MySocket *socket;
    Streaming(std::string log_name){
        this -> socket = new MySocket(log_name);
    }
    virtual ~Streaming() = default;

    void virtual toConnect(int port) = 0;
    int virtual run() = 0;
};


#endif