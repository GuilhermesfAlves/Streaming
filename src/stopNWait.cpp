#include "include/stopNWait.hpp"

StopNWait::StopNWait(string socketType) : FluxControl(socketType){
}

//não pode subir
int StopNWait::receive(){
    int status;

    while(!(status = listen()));
    
    switch (status){
    case INVALID_MESSAGE:
        respond(message -> getFrame(), T_NACK);
        return T_INEXISTENT;
    case VALID_MESSAGE:
        respond(message -> getFrame(), T_ACK);
        return message -> getType();
    }
}

//não pode subir
int StopNWait::listen(){
    char buffer[BUFFER_SIZE] = {0};
    long long start;
    int status;

    start = timestamp();
    do {
        status = message -> setMessage(socket -> collect(buffer));
    } while ((status == NOT_A_MESSAGE) && (timestamp() - start <= (DEFAULT_TIMEOUT << 2)));

    return status;
}

//não pode subir
int StopNWait::send(unsigned char type, char* msg){
    msg_t* toSend = message -> deserializeMessage(type, msg);
    do {
        socket -> post(toSend, message -> getMessageSize());
    } while(!listen());

    return confirmAck(message -> getFrame());    
}