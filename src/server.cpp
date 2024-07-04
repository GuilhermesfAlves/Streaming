#include "headers/server.hpp"


int Server::run(){
    char buffer[BUFFER_SIZE];
    Message m;
    socket -> collect(buffer);
    m.setMessage(buffer);
    cout << "Mensagem recebida: " << m.getData() << endl;

    m.deserializeMessage(T_PRINT, "Hello from server!");
    socket -> post(m.getMessage());

    return 0;
}

Server::Server() : Streaming("Server"){}
