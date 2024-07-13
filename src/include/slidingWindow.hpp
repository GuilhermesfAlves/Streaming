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
    char* isNotInWindow(char* message);
    void showWindow();
public: 
    SlidingWindow(string socketType, char operationMode);
    int empty();
    void update();
    void add(char type, const char* msg);
    void add(char type, ifstream* file);
    int getWindow();
    int getResponse();
    int dataSize();
    void flushData();
    void printData();
    int buildDataFile(char* fileName);
};

#endif