#include "include/stopNWait.hpp"

StopNWait::StopNWait(string socketType) : FluxControl(socketType){
}

int StopNWait::receive(int timeout){
    int status;

    status = listen(timeout);
    
    switch (status){
    case INVALID_MESSAGE:
        respond(message -> getFrame(), T_NACK);
        return NOT_A_MESSAGE;
    case VALID_MESSAGE:
        respond(message -> getFrame(), T_ACK);
        return message -> getType();
    default:
        return NOT_A_MESSAGE;
    }
}

int StopNWait::listen(int timeout){
    char buffer[BUFFER_SIZE] = {0};
    long long start;
    int status;
    int time;

    if (timeout == INFINIT_TIMEOUT)
        time = 0;
    else if (timeout == OPTIONAL_TIMEOUT)
        time = TIMEOUT_MILLIS << SHORTEST_TIMEOUT;
    else 
        time = TIMEOUT_MILLIS << timeout;
    
    do {
        start = timestamp();
        do {
            status = message -> setMessage(socket -> collect(buffer));
        } while ((status == NOT_A_MESSAGE) && ((!time) || (timestamp() - start <= time)));
        time <<= 1;
    } while ((status == NOT_A_MESSAGE) && (time < TIMEOUT_MILLIS << (timeout + TIMEOUT_TOLERATION)));
    
    if ((status == NOT_A_MESSAGE) && (timeout != OPTIONAL_TIMEOUT))
        throw TimeoutException(timeout);
    
    return status;
}

void StopNWait::send(unsigned char type, char* msg){
    msg_t* toSend = message -> deserializeMessage(type, msg);
    do {
        socket -> post(toSend, message -> getMessageSize());
    } while(listen(SHORT_TIMEOUT) && !confirmAck(message -> getFrame()));
}

void StopNWait::send(unsigned char type, int msg){
    char buffer[BUFFER_SIZE] = {0};
    sprintf(buffer, "%d", msg);
    send(type, buffer);
}

void StopNWait::send(unsigned char type){
    send(type, (char*)NULL);
}

int StopNWait::getDataNumber(){
    return message -> dataAtoi();
}

char* StopNWait::getDataStr(){
    return message -> getData();
}