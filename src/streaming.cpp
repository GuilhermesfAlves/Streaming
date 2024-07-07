#include "headers/streaming.hpp"

Streaming::Streaming(string log_name): window(SlidingWindow(log_name)){}

void Streaming::toConnect(char* connection){
    window.connect(if_nametoindex(connection));
}