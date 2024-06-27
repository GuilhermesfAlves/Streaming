#include "headers/client.hpp"
#include <cstdlib> // Para rand() e srand()
#include <ctime> // Para time()

#define MAX_PWD 100000
#define MIN_PWD 1000

int Client::run(){
    const char *message = "Hello, TCP Server!";
    char buffer[BUFFER_SIZE] = {0};

    std::memcpy(buffer, message, strlen(message));
    socket -> post(buffer);

    socket -> collect(buffer);

    std::cout << "Mensagem recebida: " << buffer << endl;
    return 0;
}

void Client::handshake(){
    srand(static_cast<unsigned>(time(0)));
    int random_pwd = rand() % (MAX_PWD - MIN_PWD + 1);
    char buffer[BUFFER_SIZE] = {0};

    sprintf (buffer, "%d", random_pwd);
    socket -> post(buffer);
    socket -> collect(buffer);

    if (atoi(buffer) != ++random_pwd){
        socket -> logger -> output("Handshake Failed");
        exit(EXIT_FAILURE);
    }
    strcpy(buffer, PURE_ACK);
    socket -> post(buffer);
}

Client::Client() : Streaming("Client"){}
#ifdef __DEV_MODE__

void Client::toConnect(int port){
    struct sockaddr_in servaddr = {0};

    // Configura o endereço do servidor
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conecta o socket ao endereço do servidor
    socket -> createConnection(&servaddr);
}
#endif
#ifdef __PRD_MODE__
// TODO: Client connection with raw socket 
void Client::toConnect(int port){
    return;
}
#endif