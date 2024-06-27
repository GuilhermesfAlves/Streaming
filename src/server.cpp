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
    MySocket socket = MySocket(SERVER_SOCKET_STR);
    char buffer[BUFFER_SIZE];

    socket.toConnect(SERVER_MODE);
  
    socket.collect(buffer);
    cout << "Mensagem recebida: "<< buffer << endl;

    strcpy(buffer,"Hello from server!");
    socket.post(buffer);

    return 0;
}