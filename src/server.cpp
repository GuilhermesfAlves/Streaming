#include "headers/server.hpp"



// // Estrutura da mensagem customizada
// struct CustomMessage {
//     uint16_t messageSize;  // Tamanho da mensagem em bytes
//     uint16_t messageId;    // ID da mensagem
//     uint8_t crcType;       // Tipo de CRC (por exemplo, CRC8, CRC16)
//     // Aqui você pode adicionar outros campos da sua mensagem
//     char data[512];        // Dados da mensagem
// };

int serverMethod(int port) {
    int sockfd;
    int len;
    char buffer[BUFFER_SIZE];

    // Criação do socket raw
    if (!createSocket(&sockfd)) {
        cerr << "Erro ao criar socket" << endl;
        return 1;
    }

    if (!createServerConnection(&sockfd, port)){
        cerr << "Erro ao conectar servidor" << endl;
        return 1;
    }

    if ((len = recv(sockfd, buffer, BUFFER_SIZE, 0)) < 0){
        cout << "Erro ao receber mensagem: " << len << endl;
    } else {
        buffer[len] = '\0';
        cout << "Mensagem recebida: "<< buffer << endl;
    }

    close(sockfd);
    return 0;
}