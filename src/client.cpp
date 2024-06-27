#include "headers/client.hpp"

Client::Client() : Streaming("Client"){}
#ifdef __DEV_MODE__

void Client::toConnect(int port){
    int sockfd = socket -> getSockfd();
    const char *message = "Hello from client";
    struct sockaddr_in servaddr;

    memset(&servaddr, 0, sizeof(servaddr));

    // Configura o endereço do servidor
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conecta o socket ao endereço do servidor
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        cerr << "Erro ao conectar ao servidor" << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Envia a mensagem para o servidor usando send
    if (send(sockfd, message, strlen(message), 0) < 0){
        cout << "Mensagem de inicio de conexão não enviada " << endl;
    } else {
        cout << "Mensagem de inicio de conexão enviada" << endl;
    }
}
#endif
#ifdef __PRD_MODE__
// TODO: Client connection with raw socket 
void Client::toConnect(int port){
    return;
}

#endif

int Client::run(){
    const char *message = "Hello, TCP Server!";
    char buffer[BUFFER_SIZE] = {0};

    std::memcpy(buffer, message, strlen(message));
    socket -> post(buffer);

    socket -> collect(buffer);

    std::cout << "Mensagem recebida: " << buffer << endl;
    return 0;
}
