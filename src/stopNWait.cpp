#include "include/stopNWait.hpp"

StopNWait::StopNWait(string socketType) : FluxControl(socketType){}

int StopNWait::receive(int timeout){
    int status;

    int i = 0;
    while(((status = listen(timeout << i)) == NOT_A_MESSAGE) && (++i < TIMEOUT_TOLERATION));

    if ((status == NOT_A_MESSAGE) && (timeout < OPTIONAL_TIMEOUT))
        throw TimeoutException(timeout);

    if ((status == NOT_A_MESSAGE) && (timeout >= OPTIONAL_TIMEOUT))
        return 0;

    if (!isExpectedFrame()){
        status = INVALID_MESSAGE;
    }
    
    if (status == VALID_MESSAGE){
        updateLastReceived(message -> getMessage());
        if (message -> getType() == T_PRINT)
            cout << "\t" << message -> getData() << endl;
    }

    return marshallACK(status);
}

void StopNWait::send(unsigned char type, const char* msg){
    msg_t* toSend = message -> buildMessage(type, msg);
    int status;
    int i = 0;
    addSentHistoric(toSend);
    do {
        socket -> post(toSend, msglen(toSend));
    } while((((status = listen(SHORT_TIMEOUT << i)) == NOT_A_MESSAGE) || !confirmAck(toSend -> frame)) && (++i < TIMEOUT_TOLERATION));

    if (i >= TIMEOUT_TOLERATION)
        throw TimeoutException(SHORT_TIMEOUT);

    updateLastReceived(message -> getMessage());
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
