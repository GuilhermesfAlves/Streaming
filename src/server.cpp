#include "headers/server.hpp"



// // Estrutura da mensagem customizada
// struct CustomMessage {
//     uint16_t messageSize;  // Tamanho da mensagem em bytes
//     uint16_t messageId;    // ID da mensagem
//     uint8_t crcType;       // Tipo de CRC (por exemplo, CRC8, CRC16)
//     // Aqui você pode adicionar outros campos da sua mensagem
//     char data[512];        // Dados da mensagem
// };

    Server::Server() : Streaming("Server"){}

    void Server::toConnect(int port){
        createServerConnection(this -> socket -> getSockfd(), port);
    }

    int Server::run(){
        char buffer[BUFFER_SIZE];

        socket -> collect(buffer);
        std::cout << "Mensagem recebida: "<< buffer << endl;

        std::strcpy(buffer,"Hello from server!");
        socket -> post(buffer);

        return 0;
    }
