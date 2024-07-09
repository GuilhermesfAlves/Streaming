#include "headers/server.hpp"


int Server::run(){
    // char buffer[BUFFER_SIZE];
    // Message m;
    // m.setMessage(socket -> collect(buffer));
    // cout << "Mensagem recebida: " << m.getData() << endl;

    // socket -> post((char*)m.deserializeMessage(T_PRINT, "Hello from server!"));

    while (window.collectedSize() != 6){
        window.getWindow();
        cout << "collect end "  << window.collectedSize() << endl;
    }
    cout << "fim collect, comeco envio" << endl;
    window.add(T_PRINT, "Hello from server! :)");
    while (!window.empty()){
        cout << "update" << endl;
        window.update();
    }

    return 0;
}

Server::Server() : Streaming("Server"){}
