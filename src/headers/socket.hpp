#ifndef __MY_SOCKET__
#define __MY_SOCKET__
#include "utils.hpp"

#define DEFAULT_PORT 8080
class MySocket{
private:

    int sockfd;
    
    int createSocket();
public:
    Logger* logger;

    MySocket(char* socketType);

    ~MySocket();

    void createConnection(sockaddr_in* addrToConnect);
    void toBind(sockaddr_in* addrToBind);
    int post(const char* buffer);

    int collect(char* buffer);

    int getSockfd();
};

#endif