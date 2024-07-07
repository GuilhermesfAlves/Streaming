#include "headers/client.hpp"


int Client::run(){
    // char buffer[BUFFER_SIZE] = {0};
    // Message m;

    // socket -> post((char*)m.deserializeMessage(T_PRINT, "Hello from client!"));

    // m.setMessage(socket -> collect(buffer));
    // cout << "Mensagem recebida: " << m.getData() << endl;
    
    window.add(T_PRINT, "Hello from client! :)");
    window.add(T_PRINT, "Another message :)");
    while (!window.empty()){
        cout << "update" << endl;
        window.update();
    }
    cout << "fim envio, comeco collect" << endl;
    // while (window.collectedEmpty()){
    //     cout << "collect" << endl;
    //     window.getWindow();
    // }

    return 0;
}

Client::Client() : Streaming("Client"){}