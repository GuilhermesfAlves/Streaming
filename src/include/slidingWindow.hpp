#ifndef __SLIDING_WINDOW__
#define __SLIDING_WINDOW__

#include "fluxControl.hpp"
#include <list>

#define WINDOW_SIZE 5

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
    SlidingWindow(string socketType);
    int empty();
    int collectedSize();
    void update();
    void add(char type, const char* msg);
    int getWindow();
    int getResponse();
    void flushCollected();
    void printCollected();
};

#endif