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

    MySocket(string socketType);

    ~MySocket();

    void createConnection(sockaddr_in* addrToConnect);
    void toBind(sockaddr_in* addrToBind);
    void post(char* buffer);
    void collect(char* buffer);

    int getSockfd();
};

#endif