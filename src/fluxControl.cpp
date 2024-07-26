#include "include/fluxControl.hpp"

char FluxControl::lastReceivedFrame = MAX_FRAME;
Message* FluxControl::message = NULL;
Socket* FluxControl::socket = NULL;
list<msg_t*> FluxControl::collected;
list<msg_t*> FluxControl::sent;

FluxControl::FluxControl(string socketType){
    socket = Socket::instanceOf(socketType);
    message = Message::instanceOf();
    lastReceivedFrame = MAX_FRAME;
}

void FluxControl::connect(int ifindex){
    socket -> toBind(ifindex);
    socket -> setSocketPromisc(ifindex);
    socket -> setSocketTimeout(TIMEOUT_MILLIS);
}

long long FluxControl::timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec*1000 + tp.tv_usec/1000;
}

int FluxControl::confirmAck(unsigned char frameToConfirm){
    int frame = message -> dataAtoi();

    return ((message -> getType() == T_ACK) && (frame == frameToConfirm));
}

int FluxControl::confirmAck(){
    int frame = message -> dataAtoi();

    return ((message -> getType() == T_ACK) && (frame != INEXISTENT_FRAME));
}

int FluxControl::respond(unsigned char frameToConfirm, char type){
    char buffer[BUFFER_SIZE] = {0};
    if (frameToConfirm == INEXISTENT_FRAME)
        return 0;
    if (type == T_NACK)
        frameToConfirm = (frameToConfirm + 1) & MAX_FRAME;

    sprintf(buffer, "%d", frameToConfirm);
    socket -> post(message -> deserializeMessage(type, buffer), (size_t) message -> getMessageSize());    
    addSentHistoric(message -> getMessage());
    return 1;
}

int FluxControl::getTime(int timeout){
    if (timeout == INFINIT_TIMEOUT)
        return INFINIT_TIMEOUT;
    else if (timeout >= OPTIONAL_TIMEOUT)
        return TIMEOUT_MILLIS << SHORTEST_TIMEOUT;

    return TIMEOUT_MILLIS << timeout;
}

int FluxControl::listen(int timeout){
    char buffer[BUFFER_SIZE] = {0};
    long long start;
    int status;
    int time = getTime(timeout);

    start = timestamp();
    do {
        status = alreadySent(message -> setMessage(alreadyCollected(socket -> collect(buffer))));
    } while ((status == NOT_A_MESSAGE) && ((time == INFINIT_TIMEOUT) || (timestamp() - start <= time)));
    return status;
}

int FluxControl::marshallACK(int status){
    msg_t* msg;

    switch (status){
    case VALID_MESSAGE:
        msg = message -> getMessage();
        respond(message -> getFrame(), T_ACK);
        message -> setMessage(msg);
        return message -> getType();
    case INVALID_MESSAGE:
        respond(message -> getFrame(), T_NACK);
    default:
        return NOT_A_MESSAGE;
    }
}

char* FluxControl::alreadyCollected(char* buffer){
    for (msg_t* msg : collected)
        if (!msgncmp(msg, (msg_t*) buffer, msglen(msg) - 1)){
            memset(buffer, 0, BUFFER_SIZE);
            break;
        }
    return buffer;
}

int FluxControl::alreadySent(int status){
    msg_t* toCompare =  message -> getMessage();
    if (status != VALID_MESSAGE)
        return status;
    
    for (msg_t* msg : sent)
        if (!msgncmp(msg, toCompare, msglen(msg) - 1))
            return NOT_A_MESSAGE;
            
    return VALID_MESSAGE;
}

void FluxControl::addSentHistoric(msg_t* msg){
    for (msg_t* msg : sent)
        if (!msgncmp(msg, message -> getMessage(), msglen(msg) - 1))
            return;

    if (sent.size() >= COLLECTED_HISTORIC_SIZE)
        sent.pop_back();
    sent.push_front(msg);
}

void FluxControl::addCollectHistoric(msg_t* msg){
    for (msg_t* msg : sent)
        if (!msgncmp(msg, message -> getMessage(), msglen(msg) - 1))
            return;

    if (collected.size() >= COLLECTED_HISTORIC_SIZE)
        collected.pop_back();
    collected.push_front(msg);
}

TimeoutException::TimeoutException(int timeout) : timeout(timeout) {}

const char* TimeoutException::what() const noexcept{
    int timeExploded = 0;
    for (int i = 0; i < TIMEOUT_TOLERATION; i++)
        timeExploded += TIMEOUT_MILLIS << (timeout + i);

    return strdup((messageException + to_string(timeExploded) + "ms exploded").c_str());
}
