#include "headers/slidingWindow.hpp"

void SlidingWindow::update(){
    if ((window.size() <= WINDOW_SIZE) && (!queue.empty())){
        window.push_back(queue.front());
        queue.pop_front();
    }
}

void SlidingWindow::add(msg_t* msg){
    queue.push_back(msg);
}
