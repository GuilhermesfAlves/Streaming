#include "headers/socket.hpp"


MySocket::MySocket(string socketType) : sockfd(createSocket()), logger(new Logger(socketType)){

}

MySocket::~MySocket(){
    close(this -> sockfd);
}

void MySocket::post(char* buffer){
    write(this -> sockfd, buffer, strlen(buffer));
    logger -> log(buffer);
}

void MySocket::collect(char* buffer){
    memset(buffer, 0, BUFFER_SIZE);
    read(this -> sockfd, buffer, BUFFER_SIZE);
    logger -> log(buffer);
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

int MySocket::createSocket(){
    int sk;
    if ((sk = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
        logger -> output("Build socket error");
        exit(EXIT_FAILURE);
    }
    return sk;
}