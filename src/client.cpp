#include "include/client.hpp"


int Client::run(){

    window.add(T_PRINT, "1 Hello from client! :)");
    window.add(T_PRINT, "2 Second message! :)");
    window.add(T_PRINT, "3 Third message! :)");
    window.add(T_PRINT, "4 Fourth message :)");
    window.add(T_PRINT, "5 Fifth message! :)");
    window.add(T_PRINT, "6 Sixth message! :)");
    while (!window.empty()){
        cout << "update" << endl;
        window.update();
    }
    cout << "fim envio, comeco collect" << endl;
    while (window.collectedSize() != 1){
        cout << "collect" << endl;
        window.getWindow();
    }

    return 0;
}

Client::Client() : Streaming("Client"){}