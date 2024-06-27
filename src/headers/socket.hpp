#ifndef __MY_SOCKET__
#define __MY_SOCKET__
#include "utils.hpp"

#define DEFAULT_PORT 8080
class MySocket{
private:

    int sockfd;
    std::ofstream logger;
    
    void log(const char* message, int len);

    void log(const char* message);

    void output(const char* message);

public:

    MySocket(std::string socketType);

    ~MySocket();

    void post(char* buffer);

    void collect(char* buffer);

    void toConnect(int mode);
};

#endif