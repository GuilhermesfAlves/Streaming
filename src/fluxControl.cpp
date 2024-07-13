#include "include/fluxControl.hpp"

char FluxControl::lastReceivedFrame = MAX_FRAME;
Message* FluxControl::message = NULL;
MySocket* FluxControl::socket = NULL;
list<msg_t*> FluxControl::collected;

FluxControl::FluxControl(string socketType, char operationMode){
    socket = MySocket::instanceOf(socketType);
    message = Message::instanceOf(operationMode);
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
    if (message -> getType() == T_NACK)
        cout << "NACK " << frame << endl;

    return ((message -> getType() == T_ACK) && (frame == frameToConfirm));
}

int FluxControl::confirmAck(){
    int frame = message -> dataAtoi();
    if (message -> getType() == T_NACK)
        cout << "NACK " << frame << endl;

    return ((message -> getType() == T_ACK) && (frame != INEXISTENT_FRAME));
}

int FluxControl::respond(unsigned char frameToConfirm, char type){
    char buffer[BUFFER_SIZE] = {0};
    if (frameToConfirm == INEXISTENT_FRAME)
        return 0;

    sprintf(buffer, "%d", frameToConfirm);
    socket -> post(message -> deserializeMessage(type, buffer), (size_t) message -> getMessageSize());    
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
    cout << "time: "<<time << endl;
    // do {
    start = timestamp();
    do {
        cout << "." << endl;
        status = message -> setMessage(alreadyCollected(socket -> collect(buffer)));
    } while ((status == NOT_A_MESSAGE) && ((time == INFINIT_TIMEOUT) || (timestamp() - start <= time)));

    return status;
}

int FluxControl::marshallACK(int status){
    msg_t* msg;
    switch (status){
    case VALID_MESSAGE:
        cout << ">>RECV Valid Message" << endl;
        msg = message -> getMessage();
        respond(message -> getFrame(), T_ACK);
        message -> setMessage(msg);
        return message -> getType();
    case INVALID_MESSAGE:
        cout << ">>RECV Invalid Message" << endl;
        respond(message -> getFrame(), T_NACK);
    default:
        cout << ">>RECV Not a Message" << endl;
        return NOT_A_MESSAGE;
    }
}

char* FluxControl::alreadyCollected(char* buffer){
    int exit;
    for (msg_t* m : collected){
        cout << "on collected: " << m -> body << endl;
        if (!(exit = msgncmp(m,(msg_t*) buffer, msglen(m) - 1))){
            cout << "already collected" << endl;
            memset(buffer, 0, BUFFER_SIZE);
            break;
        } else{
            cout << "new collected" << endl;
        }
    }
    return buffer;
}

void FluxControl::addCollectHistoric(msg_t* msg){
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
