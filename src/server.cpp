#include "headers/server.hpp"


int Server::run(){
    char buffer[BUFFER_SIZE];
    Message m;
    m.setMessage(socket -> collect(buffer));
    cout << "Mensagem recebida: " << m.getData() << endl;

    socket -> post((char*)m.deserializeMessage(T_PRINT, "Hello from server!"));

    return 0;
}

Server::Server() : Streaming("Server"){}
