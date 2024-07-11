#ifndef __STOP_N_WAIT__
#define __STOP_N_WAIT__

#include "fluxControl.hpp"

class StopNWait: public FluxControl{
public: 
    StopNWait(string socketType);
    int receive();
    int listen();
    int send(unsigned char type, char* msg);

};

#endif