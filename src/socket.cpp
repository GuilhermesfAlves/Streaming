#include "headers/socket.hpp"


void MySocket::log(const char* message, int len){
    logger << message << " | " << len << " | " << strerror(errno) << endl;
}

void MySocket::log(const char* message){
    logger << message << " | " << strerror(errno) << endl;
}

void MySocket::output(const char* message){
    cout << message << endl;
}

MySocket::MySocket(std::string socketType){
    if ((this -> sockfd = createSocket()) < 0){
        output("Build socket error");
    }
    this -> logger.open(socketType.append("_logger.log"));
    if (!this -> logger.is_open()){
        output("Error creating logger");
        exit(EXIT_FAILURE);
    }
}

MySocket::~MySocket(){
    close(this -> sockfd);
    this -> logger.close();
}

void MySocket::post(char* buffer){
    int len;

    if ((len = write(this -> sockfd, buffer, strlen(buffer))) < 0) {
        log("Post failed", len);
    } else {
        log("Packet posted", len);
    }
}

void MySocket::collect(char* buffer){
    int len;

    memset(buffer, 0, BUFFER_SIZE);
    if ((len = read(this -> sockfd, buffer, BUFFER_SIZE)) < 0){
        log("Collect failed", len);
    } else {
        log("Packet collected", len);
    }
}

void MySocket::toConnect(std::string socketType){
    if (!socketType.compare(CLIENT_SOCKET_STR)){
        if (!createClientConnection(this -> sockfd, DEFAULT_PORT)){
            output("Failed to connect Server");
            exit(EXIT_FAILURE);
        }
    } else if (!socketType.compare(SERVER_SOCKET_STR)){
        if (!createServerConnection(this -> sockfd, DEFAULT_PORT)){
            output("Failed to connect Server");
            exit(EXIT_FAILURE);
        }
    } else {
        output("Invalid Socket Type");
        exit(EXIT_FAILURE);
    }
}