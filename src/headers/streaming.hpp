#ifndef __STREAMING__
#define __STREAMING__
#include "utils.hpp"
#include "socket.hpp"
#include "slidingWindow.hpp"
#define PURE_ACK "Ack"

class Streaming{
public:
    MySocket *socket;
    Streaming(string log_name): socket(new MySocket(log_name)){}
    virtual ~Streaming() = default;

    void virtual toConnect(int port) = 0;
    int virtual run() = 0;
    //Similar ao Triple Handshake do TCP
    void virtual handshake() = 0;
};


#endif