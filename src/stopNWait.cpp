#include "include/stopNWait.hpp"

StopNWait::StopNWait(string socketType) : FluxControl(socketType){
}

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

int StopNWait::send(unsigned char type, char* msg){
    msg_t* toSend = message -> deserializeMessage(type, msg);
    do {
        socket -> post(toSend, message -> getMessageSize());
    } while(!listen());

    return confirmAck(message -> getFrame());    
}

int StopNWait::send(unsigned char type, int msg){
    char buffer[BUFFER_SIZE] = {0};
    sprintf(buffer, "%d", msg);
    return send(type, buffer);
}

int StopNWait::send(unsigned char type, int msg){
    return send(type, (char*)NULL);
}


int StopNWait::getDataNumber(){
    return message -> dataAtoi();
}