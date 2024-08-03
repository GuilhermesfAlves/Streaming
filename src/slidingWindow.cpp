#include "include/slidingWindow.hpp"
#include <sys/time.h>
#include <iomanip>
#include <sys/stat.h> // Para mkdir e stat
#include <sys/types.h> // Para mkdir

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

msg_t* SlidingWindow::sendWindow(){
    for (msg_t* msg: window){
        socket -> post(msg -> body, (size_t)msglen(msg));
        addSentHistoric(msg);
    }
    return window.front();
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
    msg_t* msg = message -> getMessage();

    while (!end){
        for (int count = 0, j = 0; j < TIMEOUT_TOLERATION && count < WINDOW_SIZE && !end;){
            j++;
            i = 0;
            while (((status = listen(timeout << i)) == NOT_A_MESSAGE) && (i++ < j));

            if (status == NOT_A_MESSAGE)
                continue;
            j = 0;
            if (!isExpectedFrame()){
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
            updateLastReceived(msg);
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
    int clear;
    while (!empty()){
        i = 0;
        do {
            refillWindow();
            nackList.push_front(sendWindow());
            if (nackList.size() >= NACK_TOLERATION)
                throw BadConnectionException(nackList.front(), RECEIVING_NACK);
        } while (((status = listen(timeout << i)) == NOT_A_MESSAGE) && !confirmAck() && (i++ < TIMEOUT_TOLERATION));

        if (status == NOT_A_MESSAGE)
            throw TimeoutException(timeout);

        updateLastReceived(message -> getMessage());
        frame = message -> dataAtoi();
        clear = 0;
        for (auto rit = window.rbegin(); rit != window.rend();){
            if ((!clear) && (*rit) -> frame == frame){
                if (confirmAck((*rit) -> frame)){
                    window.remove(*rit);
                    nackList.clear();
                } else { 
                    ++rit;
                }
                clear = 1;
            } else if (clear){
                window.remove(*rit);
            } else {
                ++rit;
            }
        }
    }
    return 0;
}

int SlidingWindow::tryBuildDataFile(const char* path, string fileName, unsigned int size){
    struct stat info;
    
    fileToBuild.open(path + fileName);

    if (!fileToBuild.is_open()){
        if (stat(path, &info) != 0){ 
            if (mkdir(path, 0755) != 0)
                return FILE_OPEN_FAIL;
            fileToBuild.open(path + fileName);
            if (!fileToBuild.is_open())
                return FILE_OPEN_FAIL;
        }
        return FILE_OPEN_FAIL;
    }
    fileToBuild.seekp(size - 1);
    fileToBuild.write("", 1);

    fileToBuild.close();

    if (!fileToBuild)
        return FILE_FULL_DISK;
    
    fileToBuild.open(path + fileName, ios::out | ios::binary);
    return FILE_OPEN_SUCCESS;
}