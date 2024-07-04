#ifndef __SLIDING_WINDOW__
#define __SLIDING_WINDOW__

#include "utils.hpp"
#include "message.hpp"
#include <list>

#define WINDOW_SIZE 5


class SlidingWindow{
private:
    list<msg_t*> window;
    list<msg_t*> queue;
public: 
    void update();
    void add(msg_t* msg);
};

#endif