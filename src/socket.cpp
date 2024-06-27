#include "headers/socket.hpp"


MySocket::MySocket(char* socketType) : sockfd(createSocket()), logger(new Logger(socketType)){

}

MySocket::~MySocket(){
    close(this -> sockfd);
}

int MySocket::post(const char* buffer){
    int len;

    if ((len = write(this -> sockfd, buffer, strlen(buffer))) < 0) {
        logger -> log("Post failed", len);
        return 0;
    }
    logger -> log("Packet posted", len);
    return 1;
}

int MySocket::collect(char* buffer){
    int len;

    memset(buffer, 0, BUFFER_SIZE);
    if ((len = read(this -> sockfd, buffer, BUFFER_SIZE)) < 0){
        logger -> log("Collect failed", len);
        return 0;
    }
    logger -> log("Packet collected", len);
    return 1;
}


void MySocket::createConnection(sockaddr_in* addrToConnect){
    if (connect(sockfd, (struct sockaddr *)addrToConnect, sizeof(*addrToConnect)) < 0) {
        logger -> output("Error: Can't connect");
        exit(EXIT_FAILURE);
    }
}

void MySocket::toBind(sockaddr_in* addrToConnect){
    if (bind(sockfd, (struct sockaddr*) addrToConnect, sizeof(*addrToConnect)) < 0) {
        logger -> output("Error: Can't Bind");
        exit(EXIT_FAILURE);
    }
}

int MySocket::getSockfd(){
    return this -> sockfd;
}

#ifdef __PRD_MODE__

int MySocket::createSocket(){
    int sk;
    if ((sk = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
        logger -> output("Build socket error");
        exit(EXIT_FAILURE);
    }
    return sk;
}

#endif

#ifdef __DEV_MODE__

int MySocket::createSocket(){
    int sk;
    if ((sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0){
        logger -> output("Build socket error");
        exit(EXIT_FAILURE);
    }
    return sk;
}

#endif