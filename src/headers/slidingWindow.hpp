#ifndef __SLIDING_WINDOW__
#define __SLIDING_WINDOW__

#include "socket.hpp"
#include "message.hpp"
#include <list>

#define WINDOW_SIZE 5


class SlidingWindow{
private:
    list<msg_t*> window;
    list<msg_t*> queue;
    list<msg_t*> collected;
    Message message;
    MySocket socket;
    char lastFrame;
    void sendWindow();
    void refillWindow();
    long long timestamp();
    char* isNotInWindow(char* message);
    void showWindow();
public: 
    SlidingWindow(string socketType);
    void connect(int ifindex);
    int empty();
    int collectedSize();
    void update();
    void add(char type, const char* msg);
    int getWindow();
    int getResponse();
    void respond(char type, int seq);
};

#endif