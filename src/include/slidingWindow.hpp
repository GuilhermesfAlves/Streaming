#ifndef __SLIDING_WINDOW__
#define __SLIDING_WINDOW__

#include "fluxControl.hpp"

#include <list>
#include <fstream>
#include <sstream>

#define WINDOW_SIZE 5

#define FILE_OPEN_SUCCESS 0
#define FILE_FULL_DISK 1
#define FILE_OPEN_FAIL 2

class SlidingWindow: public FluxControl{
private:
    list<msg_t*> window;
    list<msg_t*> queue;
    list<msg_t*> collected;
    void sendWindow();
    void refillWindow();
    char* isNotInWindow(char* message);
    char* alreadyCollected(char* buffer);
    void showWindow();
public: 
    SlidingWindow(string socketType, char operationMode);
    int empty();
    int collectedSize();
    void update();
    void add(char type, const char* msg);
    void add(char type, ifstream* file);
    int getWindow();
    int getResponse();
    void flushCollected();
    void printCollected();
    int buildCollectedFile(char* fileName);
};

#endif