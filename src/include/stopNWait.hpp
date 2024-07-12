#ifndef __STOP_N_WAIT__
#define __STOP_N_WAIT__

#include "fluxControl.hpp"

class StopNWait: public FluxControl{
private:
    //Escuta a entrada do socket
    int listen(int timeout);
public: 
    //atoi data
    int getDataNumber();
    //returns data
    char* getDataStr();
    StopNWait(string socketType);
    //Espera por uma única mensagem, que caso valida, envia ACK
    int receive(int timeout);
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario
    void send(unsigned char type, char* msg);
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario, msg to string
    void send(unsigned char type, int msg);
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario, msg = NULL
    void send(unsigned char type);
};

#endif