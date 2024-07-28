#include "include/slidingWindow.hpp"
#include <sys/time.h>
#include <iomanip>

SlidingWindow::SlidingWindow(string socketType) : FluxControl(socketType) {} 

int SlidingWindow::empty(){
    return (window.empty() && queue.empty());
}

void SlidingWindow::refillWindow(){
    while ((window.size() < WINDOW_SIZE) && (!queue.empty())){
        window.push_back(queue.front());
        queue.pop_front();
    }
}

int SlidingWindow::sendWindow(){
    for (msg_t* msg: window){
        socket -> post(msg -> body, (size_t)msglen(msg));
        addSentHistoric(msg);
    }
    return (int)(unsigned char) window.back() -> frame;
}

void SlidingWindow::add(char type, const char* msg){
    queue.push_back(message -> buildMessage(type, msg));
}

void SlidingWindow::add(ifstream* file) {
    char buffer[MAX_DATA_SIZE];
    int tam, sum = 0;

    memset(buffer, 0, BUFFER_SIZE);
    while ((tam = file -> readsome(buffer, MAX_DATA_SIZE)) || (file -> gcount() > 0)){
        queue.push_back(message -> buildMessage(T_DATA, buffer, tam));
        memset(buffer, 0, BUFFER_SIZE);
        sum += tam;
    }
    add(T_END_TX, NULL);
}

void SlidingWindow::receive(int timeout, unsigned int size){
    int last_status = NOT_A_MESSAGE;
    int status, end = 0;
    int i;
    unsigned char currentFrame;
    unsigned char expectedFrame;
    msg_t* msg = message -> getMessage();

    while (!end){
        for (int count = 0, j = 0; j < TIMEOUT_TOLERATION && count < WINDOW_SIZE && !end;){
            j++;
            i = 0;
            while (((status = listen(timeout + i)) == NOT_A_MESSAGE) && (i++ < j));

            if (status == NOT_A_MESSAGE)
                continue;
            j = 0;
            currentFrame = message -> getFrame();
            expectedFrame = (lastReceivedFrame + 1) & MAX_FRAME;
            if (expectedFrame != currentFrame){
                status = INVALID_MESSAGE;
            }
            if (status == INVALID_MESSAGE){
                message -> setMessage(msg);
                last_status = status;
                break;
            }
            
            msg = message -> getMessage();
            int type = message -> getType();
            switch (type){
            case T_PRINT:
                cout << "\t" << message -> getData() << endl;
                break;
            case T_DATA:
                fileToBuild.write(message -> getData(), message -> getSize());
                dataReceived += message -> getSize();
                if (dataReceived == size)
                    cout << "\r\033[K" << ">> " << fixed <<  "\x1B[32mCOMPLETE\x1B[0m" << flush;
                else
                    cout << "\r\033[K" << ">> " << fixed << setprecision(2) << setw(6) << setfill('0') << (float) dataReceived / size * 100 << "%" << flush;
                break;
            case T_END_TX:
                end = 1;
                dataReceived = 0;
                cout << endl;
                fileToBuild.close();
                break;
            default:
                break;
            }
            count++;
            addCollectHistoric(msg);
            lastReceivedFrame = currentFrame;
            last_status = status;
        }
        message -> setMessage(msg);
        marshallACK(last_status);    
    }
}

int SlidingWindow::send(int timeout){
    int status;
    int i;
    int frame;

    while (!empty()){
        i = 0;
        do {
            refillWindow();
            sendWindow();
        } while (((status = listen(timeout + i)) == NOT_A_MESSAGE) && !confirmAck() && (i++ < TIMEOUT_TOLERATION));

        if (status == NOT_A_MESSAGE)
            throw TimeoutException(timeout);

        addCollectHistoric(message -> getMessage());
        lastReceivedFrame = message -> getFrame();
        while ((!window.empty()) && (frame != (window.front() -> frame)) && (window.size() > 1))
            window.pop_front();
        
        if (confirmAck(window.front() -> frame)){
            window.pop_front();
        }
    }
    return 0;
}

int SlidingWindow::tryBuildDataFile(const char* fileName, unsigned int size){
    fileToBuild.open(fileName);

    if (!fileToBuild.is_open())
        return FILE_OPEN_FAIL;

    fileToBuild.seekp(size - 1);
    fileToBuild.write("", 1);

    fileToBuild.close();

    if (!fileToBuild)
        return FILE_FULL_DISK;
    
    fileToBuild.open(fileName, ios::out | ios::binary);
    return FILE_OPEN_SUCCESS;
}