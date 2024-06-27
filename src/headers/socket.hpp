#ifndef __MY_SOCKET__
#define __MY_SOCKET__
#include "utils.hpp"

#define DEFAULT_PORT 8080
class MySocket{
private:

    int sockfd;
    Logger* logger;
    
    void log(const char* message, int len);

    void log(const char* message);

    void output(const char* message);

    int createSocket();
public:

    MySocket(char* socketType);

    ~MySocket();

    void post(char* buffer);

    void collect(char* buffer);

    int getSockfd();
};

#endif