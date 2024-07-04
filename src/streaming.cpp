#include "headers/streaming.hpp"

Streaming::Streaming(string log_name): socket(new MySocket(log_name)){}

void Streaming::toConnect(char* connection){
    int ifindex;
    ifindex = if_nametoindex(connection);
    socket -> toBind(ifindex);
    socket -> setSocketOpt(ifindex);
}