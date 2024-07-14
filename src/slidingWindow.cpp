#include "include/slidingWindow.hpp"
#include <sys/time.h>

SlidingWindow::SlidingWindow(string socketType, char operationMode) : FluxControl(socketType, operationMode) {} 

int SlidingWindow::empty(){
    return (window.empty() && queue.empty());
}

void SlidingWindow::refillWindow(){
    while ((window.size() < WINDOW_SIZE) && (!queue.empty())){
        window.push_back(queue.front());
        queue.pop_front();
    }
}

void SlidingWindow::sendWindow(){
    for (msg_t* msg: window){
        socket -> post(msg-> body, (size_t)msglen(msg));
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

int SlidingWindow::receive(int timeout){
    int last_status = NOT_A_MESSAGE;
    int status;
    int i;
    unsigned char currentFrame;
    unsigned char expectedFrame;
    msg_t* msg;

    for (int j = 0; j < TIMEOUT_TOLERATION; j++){
        i = 0;
        while (((status = listen(timeout)) == NOT_A_MESSAGE) && (++i < TIMEOUT_TOLERATION)) ;
        if (status == NOT_A_MESSAGE)
            continue;

        currentFrame = message -> getFrame();
        expectedFrame = (lastReceivedFrame + 1) & MAX_FRAME;
        if (expectedFrame != currentFrame)
            status = INVALID_MESSAGE;

        if (status == INVALID_MESSAGE)
            break;
        
        msg = message -> getMessage();
        data.push_back(message -> getMessage());
        addCollectHistoric(message -> getMessage());
        lastReceivedFrame = currentFrame;
        last_status = status;
    };
    message -> setMessage(msg);
    return marshallACK(last_status);
}

int SlidingWindow::send(int timeout){
    int status;
    int i;
    int frame;

    if (window.empty())
        refillWindow();
    while (!window.empty()){
        i = 0;
        do {
            refillWindow();
            sendWindow();
        } while (((status = listen(timeout + i)) == NOT_A_MESSAGE) && !confirmAck() && (++i < TIMEOUT_TOLERATION));

        if (status == NOT_A_MESSAGE)
            throw TimeoutException(timeout);

        addCollectHistoric(message -> getMessage());
        lastReceivedFrame = message -> getFrame();
        while ((!window.empty()) && (frame != (window.front() -> frame)) && (window.size() > 1)){
            window.pop_front();
        }

        if (confirmAck(window.front() -> frame)){
            window.pop_front();
            continue;
        }
    }
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