#include "headers/slidingWindow.hpp"
#include <sys/time.h>

SlidingWindow::SlidingWindow(string socketType) : socket(socketType){
    lastFrame = MAX_FRAME;
} 

int SlidingWindow::empty(){
    return (window.empty() && queue.empty());
}

int SlidingWindow::collectedSize(){
    return collected.size();
}

void SlidingWindow:: showWindow(){
    for (msg_t* m : window)
        cout << "in window: " << m -> m << endl;
}

void SlidingWindow::update(){
    refillWindow();
    sendWindow();
    showWindow();
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
    queue.push_back(message.deserializeMessage(type, msg));
}

void SlidingWindow::connect(int ifindex){
    socket.toBind(ifindex);
    socket.setSocketPromisc(ifindex);
}

int SlidingWindow::getWindow(){
    unsigned char currentFrame;
    char buffer[BUFFER_SIZE];
    long long start;
    start = timestamp();

    socket.setSocketTimeout(DEFAULT_TIMEOUT);
    do {
        message.setMessage(socket.collect(buffer));
        currentFrame = message.getFrame();
        if ((currentFrame == INEXISTENT_FRAME) || (currentFrame == lastFrame))
            continue;
        cout << (int) currentFrame << " coletado" << endl;
        if (((lastFrame + 1) & MAX_FRAME) != currentFrame){
            respond(T_NACK, (lastFrame + 1) & MAX_FRAME);
            return 0;
        }
        
        collected.push_back((msg_t*)message.getMessage());
        lastFrame = currentFrame;
    } while (timestamp() - start <= (DEFAULT_TIMEOUT << 1));
    respond(T_ACK, currentFrame);

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
    char frame = INEXISTENT_FRAME;
    do {
        cout << "last frame: " << (int) frame << endl;
        cout << "listening" << endl;
        message.setMessage(isNotInWindow(socket.collect(buffer)));
        showWindow();
        cout << "end1" << endl;
    } while (((frame = myAtoi(message.getData())) == INEXISTENT_FRAME) && (timestamp() - start <= (DEFAULT_TIMEOUT << 1)));

    cout << "time exploded: " << (bool) (start - timestamp() > (DEFAULT_TIMEOUT << 1)) << endl;
    cout << "frame: " << (int) frame << endl;
    if (frame == INEXISTENT_FRAME)
        return 0;
    cout << "not inexistent" << endl;
    cout << "received type: " << (int) message.getType() << endl;
    cout << "window front frame: " << (int) window.front() -> frame << endl;
    while ((!window.empty()) && (frame != window.front() -> frame)){
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
    showWindow();
    return 0;
}

void SlidingWindow::respond(char type, int frame){
    char buffer[BUFFER_SIZE] = {0};
    if (frame == INEXISTENT_FRAME)
        return;

    cout << "responding type: " << (int) type << " / frame: " << frame << endl;
    sprintf(buffer, "%d", frame);
    socket.post((char*)message.deserializeMessage(type, buffer));    
}

long long SlidingWindow::timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec*1000 + tp.tv_usec/1000;
}

char* SlidingWindow::isNotInWindow(char* message){
    for (msg_t* msg : window){
        cout << "comparing: " << message << " com: " << msg -> m << endl;
        if (!strcmp(msg -> m, message)){
            memset(message, 0, BUFFER_SIZE);
            break;
        }
    }
    return message;
}
