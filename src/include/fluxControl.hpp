#ifndef __FLUX_CONTROL__
#define __FLUX_CONTROL__

#include "socket.hpp"
#include "message.hpp"

class FluxControl{
protected:
    static Message* message;
    static MySocket* socket;
    static char lastFrame;
    long long timestamp();
public: 
    FluxControl(string socketType);
    void connect(int ifindex);
    int confirmAck(unsigned char frameToConfirm);
    int respond(unsigned char frameToConfirm, char type);
};

#endif