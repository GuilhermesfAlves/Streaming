#ifndef __STOP_N_WAIT__
#define __STOP_N_WAIT__

#include "fluxControl.hpp"

class StopNWait: public FluxControl{
public: 
    int getDataNumber();
    StopNWait(string socketType);
    //Espera por uma única mensagem, que caso valida, envia ACK
    int receive();
    //Escuta a entrada do socket
    int listen();
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario
    int send(unsigned char type, char* msg);
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario, msg to string
    int send(unsigned char type, int msg);
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario, msg = NULL
    int send(unsigned char type);
};

#endif