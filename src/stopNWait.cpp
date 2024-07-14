#include "include/stopNWait.hpp"

StopNWait::StopNWait(string socketType, char operationMode) : FluxControl(socketType, operationMode){
}

int StopNWait::receive(int timeout){
    int status;

    int i = 0;
    while(((status = listen(timeout << i)) == NOT_A_MESSAGE) && (++i < TIMEOUT_TOLERATION));

    if ((status == NOT_A_MESSAGE) && (timeout < OPTIONAL_TIMEOUT))
        throw TimeoutException(timeout);
    
    unsigned char currentFrame = message -> getFrame();
    unsigned char expectedFrame = (lastReceivedFrame + 1) & MAX_FRAME;
    if (expectedFrame != currentFrame){
        status = INVALID_MESSAGE;
    }
    if (status == VALID_MESSAGE){
        addCollectHistoric(message -> getMessage());
        lastReceivedFrame = currentFrame;
    }

    return marshallACK(status);
}

void StopNWait::send(unsigned char type, char* msg){
    msg_t* toSend = message -> deserializeMessage(type, msg);
    int status;
    int i = 0;
    do {
        socket -> post(toSend, msglen(toSend));
    } while(((status = listen(SHORT_TIMEOUT << i)) == NOT_A_MESSAGE) && !confirmAck(toSend -> frame) && (++i < TIMEOUT_TOLERATION));

    if (status == NOT_A_MESSAGE)
        throw TimeoutException(SHORT_TIMEOUT);

    addCollectHistoric(message -> getMessage());
    lastReceivedFrame = message -> getFrame();
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
