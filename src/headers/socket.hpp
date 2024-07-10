#ifndef __MY_SOCKET__
#define __MY_SOCKET__
#include "utils.hpp"
#include "logger.hpp"

class MySocket{
private:
    int sockfd;
    int createSocket();
    char *lastSent;
public:
    Logger* logger;
    MySocket(string socketType);
    ~MySocket();
    //bind socket to read and write on <lo> or <enp3s0>
    void toBind(int ifindex);
    //set socket as PROMISC
    void setSocketPromisc(int ifindex);
    void setSocketTimeout(int timeoutMillis);
    void post(const void* buffer, size_t len);
    char* collect(char* buffer);
};

#endif