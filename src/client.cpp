#include "headers/client.hpp"
#include <cstdlib> // Para rand() e srand()
#include <ctime> // Para time()

#define MAX_PWD 100000
#define MIN_PWD 1000

int Client::run(){
    char buffer[BUFFER_SIZE] = {0};
    Message m;

    m.deserializeMessage(T_PRINT, "Hello, TCP Server!");
    socket -> post(m.getMessage());

    socket -> collect(buffer);
    m.setMessage(buffer);
    cout << "Mensagem recebida: " << m.getData() << endl;
    return 0;
}

void Client::handshake(){
    srand(static_cast<unsigned>(time(0)));
    int random_pwd = rand() % (MAX_PWD - MIN_PWD + 1);
    char buffer[BUFFER_SIZE] = {0};
    Message m;
    sprintf (buffer, "%d", random_pwd);
    m.deserializeMessage(T_PRINT, buffer);
    socket -> post(m.getMessage());
    socket -> collect(buffer);
    m.setMessage(buffer);
    if (atoi(m.getData()) != ++random_pwd){
        socket -> logger -> output("Handshake Failed");
        exit(EXIT_FAILURE);
    }
    m.deserializeMessage(T_ACK, NULL);
    socket -> post(m.getMessage());
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