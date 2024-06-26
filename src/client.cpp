#include "headers/client.hpp"


int clientMethod(int port) {
    const char *message = "Hello, TCP Server!";
    char buffer[1024];
    int sockfd;

    // Conecta ao servidor
    if (!createSocket(&sockfd)) {
        cerr << "Erro ao conectar" << endl;
        exit(EXIT_FAILURE);
    }

    if (!createClientConnection(&sockfd, port)){
        cerr << "Erro ao conectar" << endl;
        exit(EXIT_FAILURE);
    }

    size_t packet_len = 50; // 2 bytes header, message length, 1 byte checksum

    memcpy(buffer, message, 20);
    int len = 0;
    while (len == 0){
        if ((len = send(sockfd, buffer, packet_len, 0)) < 0) {
            cerr << "Sendto error " << len << endl;
        } else {
            cout << "Packet Sent, size: " << len << " | message: " << buffer << endl;
        }
    }

    close(sockfd);
    return 0;
}