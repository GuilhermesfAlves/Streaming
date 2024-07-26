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
    ofstream fileToBuild;
    unsigned int dataReceived = 0;
    int sendWindow();
    void refillWindow();
public: 
    SlidingWindow(string socketType);
    int empty();
    void add(char type, const char* msg);
    void add(ifstream* file);
    void receive(int timeout, int size);
    int send(int timeout);
    int tryBuildDataFile(const char* fileName, unsigned int size);
};

#endif