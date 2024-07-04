#include "headers/socket.hpp"
#define TX "Tx"
#define RX "Rx"

MySocket::MySocket(string socketType) : sockfd(createSocket()), logger(new Logger(socketType)){}

MySocket::~MySocket(){
    close(this -> sockfd);
}

void MySocket::post(char* buffer){
    write(this -> sockfd, buffer, strlen(buffer));
    logger -> log(buffer, TX);
}

void MySocket::collect(char* buffer){
    memset(buffer, 0, BUFFER_SIZE);
    read(this -> sockfd, buffer, BUFFER_SIZE);
    logger -> log(buffer, RX);
}

void MySocket::toBind(int ifindex){
    sockaddr_ll endereco;

    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(sockfd, (struct sockaddr*) &endereco, sizeof(endereco)) < 0) {
        logger -> output("Error: Can't Bind");
        exit(EXIT_FAILURE);
    }
}

int MySocket::getSockfd(){
    return this -> sockfd;
}

int MySocket::createSocket(){
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
        logger -> output("Build socket error");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}