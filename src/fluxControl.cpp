#include "include/fluxControl.hpp"

unsigned char FluxControl::lastReceivedFrame = MAX_FRAME;
Message* FluxControl::message = NULL;
Socket* FluxControl::socket = NULL;
list<msg_t*> FluxControl::collected;
list<msg_t*> FluxControl::sent;
list<msg_t*> FluxControl::nackList;

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
    return ((message -> getType() == T_ACK) && (message -> dataAtoi() == frameToConfirm));
}

int FluxControl::confirmAck(){
    return (((message -> getType() == T_ACK) || (message -> getType() == T_NACK)) && (message -> dataAtoi() != INEXISTENT_FRAME));
}

int FluxControl::respond(unsigned char frameToConfirm, char type){
    char buffer[BUFFER_SIZE] = {0};

    if (frameToConfirm == INEXISTENT_FRAME)
        return 0;

    if (type == T_NACK){
        nackList.push_front(message -> getMessage());
        if (nackList.size() > NACK_TOLERATION)
            throw BadConnectionException(message -> getMessage(), SENDING_NACK);
        frameToConfirm = (frameToConfirm + 1) & MAX_FRAME;
    } else {
        nackList.clear();
    }
    sprintf(buffer, "%d", frameToConfirm);
    socket -> post(message -> buildMessage(type, buffer), (size_t) message -> getMessageSize());
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
        status = message -> setMessage(inContext(socket -> collect(buffer)));
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

char* FluxControl::inContext(char* buffer){
    for (msg_t* msg : collected)
        if (!msgncmp(msg, (msg_t*) buffer, msglen(msg) - 1)){
            memset(buffer, 0, BUFFER_SIZE);
            break;
        }
    for (msg_t* msg : sent)
        if (!msgncmp(msg, (msg_t*)buffer, msglen(msg) - 1)){
            memset(buffer, 0, BUFFER_SIZE);
            break;
        }
    return buffer;
}

void FluxControl::addSentHistoric(msg_t* newMsg){
    for (msg_t* msg : sent)
        if (!msgncmp(msg, newMsg, msglen(msg) - 1))
            return;

    if (sent.size() >= COLLECTED_HISTORIC_SIZE)
        sent.pop_back();
    sent.push_front(newMsg);
}

void FluxControl::addCollectHistoric(msg_t* newMsg){
    for (msg_t* msg : collected)
        if (!msgncmp(msg, newMsg, msglen(msg) - 1))
            return;

    if (collected.size() >= COLLECTED_HISTORIC_SIZE)
        collected.pop_back();
    collected.push_front(newMsg);
}

void FluxControl::flushHistoric(){
    collected.clear();
    sent.clear();
}

void FluxControl::updateLastReceived(msg_t * msg){
    addCollectHistoric(msg);
    lastReceivedFrame = msg -> frame;
}

bool FluxControl::isExpectedFrame(){
    return (message -> getFrame() == ((lastReceivedFrame + 1) & MAX_FRAME));
}

TimeoutException::TimeoutException(int timeout) : timeout(timeout) {}

const char* TimeoutException::what() const noexcept{
    int timeExploded = 0;
    for (int i = 0; i < TIMEOUT_TOLERATION; i++)
        timeExploded += TIMEOUT_MILLIS << (timeout + i);

    return strdup((messageException + to_string(timeExploded) + "ms exploded").c_str());
}

BadConnectionException::BadConnectionException(msg_t* badMessage, bool isSending) : badMessage(badMessage), isSending(isSending){}

const char* BadConnectionException::what() const noexcept{
    return strdup((messageException + ((isSending)?"Can't send: ":"Malformed Message: ")\
    + msgToString(badMessage)).c_str());
}