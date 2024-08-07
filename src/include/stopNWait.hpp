#ifndef __STOP_N_WAIT__
#define __STOP_N_WAIT__

#include "fluxControl.hpp"


class StopNWait: public FluxControl{
public: 
    //atoi data
    int getFileSize();

    tm getFileData();
    //returns data
    char* getDataStr();
    StopNWait(string socketType);
    //Espera por uma única mensagem, que caso valida, envia ACK
    int receive(int timeout);
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario
    void send(unsigned char type, const char* msg);
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario, msg to string
    void send(unsigned char type, int msg);
    //Envia mensagem, retorna 1 se o ack for confirmado, 0 caso contrario, msg = NULL
    void send(unsigned char type);

    void send(ifstream* file, string path);
};

#endif