#include "include/fluxControl.hpp"

char FluxControl::lastFrame = MAX_FRAME;
Message* FluxControl::message = new Message();
MySocket* FluxControl::socket = NULL;

FluxControl::FluxControl(string socketType){
    socket = new MySocket(socketType);
    lastFrame = MAX_FRAME;
    cout << "inicializado" << endl;
}

void FluxControl::connect(int ifindex){
    socket -> toBind(ifindex);
    socket -> setSocketPromisc(ifindex);
    socket -> setSocketTimeout(DEFAULT_TIMEOUT);
}

long long FluxControl::timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec*1000 + tp.tv_usec/1000;
}

int FluxControl::confirmAck(unsigned char frameToConfirm){
    int frame = message -> dataAtoi();
    lastFrame = message -> getFrame();

    return ((message -> getType() == T_ACK) && (frame == frameToConfirm));
}

int FluxControl::respond(unsigned char frameToConfirm, char type){
    char buffer[BUFFER_SIZE] = {0};
    if (frameToConfirm == INEXISTENT_FRAME)
        return 0;

    sprintf(buffer, "%d", frameToConfirm);
    socket -> post(message -> deserializeMessage(type, buffer), (size_t) message -> getMessageSize());    
    return 1;
}