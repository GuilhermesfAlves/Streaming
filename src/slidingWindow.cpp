#include "headers/slidingWindow.hpp"
#include <sys/time.h>

SlidingWindow::SlidingWindow(string socketType) : socket(socketType){
    lastCollected = T_INEXISTENT;
} 

int SlidingWindow::empty(){
    return (window.empty() && queue.empty());
}

int SlidingWindow::collectedEmpty(){
    return collected.empty();
}

void SlidingWindow::update(){
    refillWindow();
    sendWindow();
    if (!window.empty()){
        getResponse();
    }
    cout << "end getResponse" << endl;
}

void SlidingWindow::refillWindow(){
    while ((window.size() <= WINDOW_SIZE) && (!queue.empty())){
        cout << "refilling window" << endl;
        window.push_back(queue.front());
        queue.pop_front();
    }
}

void SlidingWindow::sendWindow(){
    for (msg_t* m : window){
        cout << "sending window" << endl;
        socket.post(m -> m);
    }
}

void SlidingWindow::add(char type, const char* msg){
    message.deserializeMessage(type, msg);
    
    queue.push_back((msg_t*)message.getMessage());
}

void SlidingWindow::connect(int ifindex){
    socket.toBind(ifindex);
    socket.setSocketPromisc(ifindex);
}

int SlidingWindow::getWindow(){
    unsigned char currentCollected;
    char buffer[BUFFER_SIZE];
    long long start = timestamp();
    start = timestamp();

    socket.setSocketTimeout(DEFAULT_TIMEOUT);
    do {
        message.setMessage(socket.collect(buffer));
        currentCollected = message.getSeq();
        if (currentCollected == INEXISTENT_FRAME)
            continue;
        cout << (int) currentCollected << " coletado" << endl;
        if ((lastCollected != T_INEXISTENT) && (((lastCollected + 1) & MAX_FRAME) != currentCollected)){
            respond(T_NACK, (lastCollected + 1) & MAX_FRAME);
            return 0;
        }
        
        collected.push_back((msg_t*)message.getMessage());
        lastCollected = currentCollected;
    } while (timestamp() - start <= (DEFAULT_TIMEOUT << 1));
    respond(T_ACK, currentCollected);

    return 1;
}

int myAtoi(char* str){

    for (int i = 0; str[i] != '\0'; i++){
        if ((str[i] < '0') || (str[i] > '9'))
            return INEXISTENT_FRAME;
    }
    return atoi(str);
}

int SlidingWindow::getResponse(){
    char buffer[BUFFER_SIZE] = {0};
    long long start;

    socket.setSocketTimeout(DEFAULT_TIMEOUT);
    start = timestamp();
    char seq = INEXISTENT_FRAME;
    do {
        cout << "last seq: " << (int) seq << endl;
        cout << "listening" << endl;
        message.setMessage(isNotInWindow(socket.collect(buffer)));
        cout << "end1" << endl;
    } while (((seq = myAtoi(message.getData())) == INEXISTENT_FRAME) && (timestamp() - start <= (DEFAULT_TIMEOUT << 1)));

    cout << "time exploded: " << (bool) (start - timestamp() > (DEFAULT_TIMEOUT << 1)) << endl;
    cout << "seq: " << (int) seq << endl;
    if (seq == INEXISTENT_FRAME)
        return 0;
    cout << "not inexistent" << endl;
    cout << "received type: " << (int) message.getType() << endl;
    cout << "window front seq: " << (int) window.front() -> seq << endl;
    while ((!window.empty()) && (seq != window.front() -> seq)){
        cout << "pop" << endl;
        window.pop_front();
    }
    cout << "front popped" << endl;
    char type;
    if ((type = message.getType()) == T_ACK){
        cout << "ACK confirmed" << endl;
        window.pop_front();
        return 1;
    }
    cout << "Not Ack" << endl;
    return 0;
}

void SlidingWindow::respond(char type, int seq){
    char buffer[BUFFER_SIZE] = {0};
    if (seq == INEXISTENT_FRAME)
        return;

    cout << "responding type: " << (int) type << " / seq: " << seq << endl;
    sprintf(buffer, "%d", seq);
    socket.post((char*)message.deserializeMessage(type, buffer));    
}

long long SlidingWindow::timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec*1000 + tp.tv_usec/1000;
}

char* SlidingWindow::isNotInWindow(char* message){
    for (msg_t* msg : window){
        if (!strcmp(msg -> m, message)){
            memset(message, 0, BUFFER_SIZE);
            return message;
        }
    }
    return message;
}
