#include "headers/client.hpp"


int Client::run(){
    char buffer[BUFFER_SIZE] = {0};
    Message m;

    socket -> post((char*)m.deserializeMessage(T_PRINT, "Hello from client!"));

    m.setMessage(socket -> collect(buffer));
    cout << "Mensagem recebida: " << m.getData() << endl;
    return 0;
}

Client::Client() : Streaming("Client"){}