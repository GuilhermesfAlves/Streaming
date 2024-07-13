#ifndef __SLIDING_WINDOW__
#define __SLIDING_WINDOW__

#include "fluxControl.hpp"

#include <fstream>
#include <sstream>

#define WINDOW_SIZE 5

class SlidingWindow: public FluxControl{
private:
    list<msg_t*> window;
    list<msg_t*> queue;
    list<msg_t*> data;
    void sendWindow();
    void refillWindow();
    void showWindow();
public: 
    SlidingWindow(string socketType, char operationMode);
    int empty();
    void add(char type, const char* msg);
    void add(char type, ifstream* file);
    int receive(int timeout);
    int send(int timeout);
    int dataSize();
    void flushData();
    void printData();
    int buildDataFile(char* fileName);
};

#endif