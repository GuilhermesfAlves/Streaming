#include "headers/server.hpp"


int Server::run(){

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