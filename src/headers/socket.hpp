#ifndef __MY_SOCKET__
#define __MY_SOCKET__
#include "utils.hpp"

class MySocket{
private:
    int sockfd;
    int createSocket();
    char *lastSent;
public:
    Logger* logger;
    MySocket(string socketType);
    ~MySocket();
    void toBind(int ifindex);
    void post(char* buffer);
    void collect(char* buffer);
    int getSockfd();
};

#endif