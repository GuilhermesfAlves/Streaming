#include "include/streaming.hpp"

Streaming::Streaming(string log_name, char operationMode): window(log_name, operationMode), single(log_name, operationMode){}

void Streaming::toConnect(char* connection){
    FluxControl::connect(if_nametoindex(connection));
}