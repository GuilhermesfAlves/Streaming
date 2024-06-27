#include "headers/client.hpp"

    Client::Client() : Streaming("Client"){}

    void Client::toConnect(int port){
        createClientConnection(this -> socket -> getSockfd(), port);
    }

    int Client::run(){
        const char *message = "Hello, TCP Server!";
        char buffer[BUFFER_SIZE] = {0};

        std::memcpy(buffer, message, strlen(message));
        socket -> post(buffer);

        socket -> collect(buffer);

        std::cout << "Mensagem recebida: " << buffer << endl;
        return 0;
    }
