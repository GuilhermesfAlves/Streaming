#include "headers/client.hpp"


int Client::run(){
    char buffer[BUFFER_SIZE] = {0};
    Message m;

    m.deserializeMessage(T_PRINT, "Hello, TCP Server!");
    socket -> post(m.getMessage());

    socket -> collect(buffer);
    socket -> collect(buffer);
    cout << buffer;
    m.setMessage(buffer);
    cout << "Mensagem recebida: " << m.getData() << endl;
    return 0;
}

Client::Client() : Streaming("Client"){}