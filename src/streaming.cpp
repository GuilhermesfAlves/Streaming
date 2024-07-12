#include "include/streaming.hpp"

Streaming::Streaming(string log_name): window(log_name), single(log_name){}

void Streaming::toConnect(char* connection){
    FluxControl::connect(if_nametoindex(connection));
}