#include "include/slidingWindow.hpp"
#include <sys/time.h>

SlidingWindow::SlidingWindow(string socketType, char operationMode) : FluxControl(socketType, operationMode) {
} 

int SlidingWindow::empty(){
    return (window.empty() && queue.empty());
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
    for (msg_t* msg: window){
        cout << "sending window: " << msg-> m << "." << endl;
        socket -> post(msg-> m, (size_t)msglen(msg));
    }
}

void SlidingWindow::add(char type, const char* msg){
    queue.push_back(message -> deserializeMessage(type, msg));
}

void SlidingWindow::add(char type, ifstream* file){
    char buffer[MAX_DATA_SIZE] = {0};
    while (!file -> eof()){
        file -> read(buffer, MAX_DATA_SIZE);
        queue.push_back(message -> deserializeMessage(type, buffer));
    }
}

int SlidingWindow::getWindow(){
    unsigned char currentFrame;
    char buffer[BUFFER_SIZE];
    long long start;
    int before = lastFrame;
    start = timestamp();

    do {
        message -> setMessage(alreadyCollected(socket -> collect(buffer)));
        currentFrame = message -> getFrame();
        if ((currentFrame == INEXISTENT_FRAME) || (currentFrame == lastFrame))
            continue;

        if (((lastFrame + 1) & MAX_FRAME) != currentFrame){
            respond((lastFrame + 1) & MAX_FRAME, T_NACK);
            return 0;
        }
        
        addCollectHistoric(message -> getMessage());
        lastFrame = currentFrame;
    } while (timestamp() - start <= (DEFAULT_TIMEOUT << 2));

    if (lastFrame == before)
        return 0;

    respond(lastFrame, T_ACK);

    return 1;
}

int SlidingWindow::getResponse(){
    char buffer[BUFFER_SIZE] = {0};
    long long start;

    start = timestamp();
    unsigned char frame = INEXISTENT_FRAME;
    do {
    //    cout << "last frame: " << (int) frame << endl;
    //    cout << "listening" << endl;
        message -> setMessage(isNotInWindow(socket -> collect(buffer)));
        showWindow();
    //    cout << "end1" << endl;
    } while (((frame = message -> dataAtoi()) == INEXISTENT_FRAME) && (timestamp() - start <= (DEFAULT_TIMEOUT << 2)));

//    cout << "time exploded: " << (bool) (start - timestamp() > (DEFAULT_TIMEOUT << 1)) << endl;
//    cout << "frame: " << (int) frame << endl;
    if (frame == INEXISTENT_FRAME)
        return 0;
//    cout << "not inexistent" << endl;
//    cout << "received type: " << (int) message -> getType() << endl;
//    cout << "window front frame: " << (int) window.front() -> frame << endl;
//    cout << "Get data: " << message -> getData() << endl;
//    cout << !window.empty() << endl;
//    cout << (bool)((unsigned char)frame != (unsigned char)(window.front() -> frame)) << endl;
//    cout << (int)(unsigned char)frame << endl;
//    cout << (int)(unsigned char)window.front() -> frame << endl;
    lastFrame = message -> getFrame();
    while ((!window.empty()) && (frame != (window.front() -> frame)) && (window.size() > 1)){
        // cout << "pop" << endl;
        window.pop_front();
    }
    // cout << "front popped" << endl;
    if (confirmAck(window.front() -> frame)){
        cout << "ACK confirmed" << endl;
        window.pop_front();
        return 1;
    }

    showWindow();
    return 0;
}

int SlidingWindow::dataSize(){
    return data.size();
}

void SlidingWindow::flushData(){
    data.clear();
}   

void SlidingWindow::printData(){
    for (msg_t* m : data)
        cout << m -> data << endl;
}

int SlidingWindow::buildDataFile(char* fileName){
    ofstream fileToBuild(fileName);

    if (!fileToBuild.is_open())
        return FILE_OPEN_FAIL;

    for (msg_t* m : data){
        fileToBuild << m -> data;
        if (fileToBuild.fail()){
            fileToBuild.close();
            return FILE_FULL_DISK;
        }
    }
    fileToBuild.close();
    return FILE_OPEN_SUCCESS;
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