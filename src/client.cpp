#include "headers/client.hpp"


int clientMethod(int port) {
    const char *message = "Hello, TCP Server!";
    char buffer[BUFFER_SIZE];
    MySocket sockfd = MySocket(CLIENT_SOCKET_STR);

    sockfd.toConnect(CLIENT_MODE);

    memcpy(buffer, message, strlen(message));
    sockfd.post(buffer);

    sockfd.collect(buffer);

    cout << "Mensagem recebida: "<< buffer << endl;
    sockfd.~MySocket();
    return 0;
}