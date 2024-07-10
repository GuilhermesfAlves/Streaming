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
    // showWindow();
    if (!window.empty()){
        getResponse();
    }
    cout << "end getResponse" << endl;
}

void SlidingWindow::refillWindow(){
    while ((window.size() < WINDOW_SIZE) && (!queue.empty())){
        cout << "refilling window" << endl;
        window.push_back(queue.front());
        queue.pop_front();
    }
}

void SlidingWindow::sendWindow(){
    for (msg_t* m : window){
        cout << "sending window: " << m -> m << "." << endl;
        socket.post(m -> m, (size_t) m -> size + 4);
    }
}

void SlidingWindow::add(char type, const char* msg){
    queue.push_back(message.deserializeMessage(type, msg));
}

void SlidingWindow::connect(int ifindex){
    socket.toBind(ifindex);
    socket.setSocketPromisc(ifindex);
    socket.setSocketTimeout(DEFAULT_TIMEOUT);
}

char* SlidingWindow::alreadyCollected(char* buffer){
    for (msg_t* m : collected)
        if (!strcmp(m -> m, buffer)){
            memset(buffer, 0, BUFFER_SIZE);
            break;
        }

    return buffer;
}

int SlidingWindow::getWindow(){
    unsigned char currentFrame;
    char buffer[BUFFER_SIZE];
    long long start;
    int before = lastFrame;
    start = timestamp();

    do {
        message.setMessage(alreadyCollected(socket.collect(buffer)));
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
    } while (timestamp() - start <= (DEFAULT_TIMEOUT << 2));

    if (lastFrame == before)
        return 0;

    respond(T_ACK, lastFrame);

    return 1;
}

int SlidingWindow::getResponse(){
    char buffer[BUFFER_SIZE] = {0};
    long long start;

    start = timestamp();
    char frame = INEXISTENT_FRAME;
    do {
    //    cout << "last frame: " << (int) frame << endl;
    //    cout << "listening" << endl;
        message.setMessage(isNotInWindow(socket.collect(buffer)));
        showWindow();
    //    cout << "end1" << endl;
    } while (((frame = message.dataAtoi()) == INEXISTENT_FRAME) && (timestamp() - start <= (DEFAULT_TIMEOUT << 2)));

//    cout << "time exploded: " << (bool) (start - timestamp() > (DEFAULT_TIMEOUT << 1)) << endl;
//    cout << "frame: " << (int) frame << endl;
    if (frame == INEXISTENT_FRAME)
        return 0;
//    cout << "not inexistent" << endl;
//    cout << "received type: " << (int) message.getType() << endl;
//    cout << "window front frame: " << (int) window.front() -> frame << endl;
//    cout << "Get data: " << message.getData() << endl;
//    cout << !window.empty() << endl;
//    cout << (bool)((unsigned char)frame != (unsigned char)(window.front() -> frame)) << endl;
//    cout << (int)(unsigned char)frame << endl;
//    cout << (int)(unsigned char)window.front() -> frame << endl;
    lastFrame = message.getFrame();
    while ((!window.empty()) && (bool)((unsigned char)frame != (unsigned char)(window.front() -> frame)) && (window.size() > 1)){
        // cout << "pop" << endl;
        window.pop_front();
    }
    // cout << "front popped" << endl;
    char type;
    if (((type = message.getType()) == T_ACK) && (bool)((unsigned char)frame == (unsigned char)(window.front() -> frame))){
        cout << "ACK confirmed" << endl;
        window.pop_front();
        return 1;
    }
    cout << "Not Ack" << endl;
    if (type == T_NACK){
        cout << "NACK confirmed" << endl;
    }

    showWindow();
    return 0;
}

void SlidingWindow::respond(char type, int frame){
    char buffer[BUFFER_SIZE] = {0};
    if (frame == INEXISTENT_FRAME)
        return;

    cout << "responding type: " << (int) type << " / frame: " << frame << endl;
    sprintf(buffer, "%d", frame);
    char* respond = (char*)message.deserializeMessage(type, buffer);
    socket.post(respond, (size_t) message.getMessageSize());    
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
