#include "include/server.hpp"


int Server::run(){
    int action = 0;

    do {
        action = single.receive();
        switch (action){
        case T_DOWNLOAD:
            //confere se o arquivo referenciado na mensagem existe,
            // se sim, envia numa mensagem unica o tamanho como T_FILE_DESCRIPTOR, em seguida em janela os dados como T_DATA
            // caso contrário, envia T_ERROR com o numero 2, como não encontrado
            
            //caso no meio da transmissão dos dados o cliente resolva parar de receber ou
            //envie o erro de T_ERROR 3, de disco cheio, deve-se parar a transmissão
            break;
        case T_ERROR:
            break;
        case T_END_TX:
            return 0;
        default:
            break;
        }
    } while (action == T_INEXISTENT);
    

    // while (window.collectedSize() != 5){
    //     window.getWindow();
    //     cout << "collect end "  << window.collectedSize() << endl;
    // }
    // cout << "fim collect, comeco envio" << endl;
    // window.add(T_PRINT, "Hello from server! :)");
    // while (!window.empty()){
    //     cout << "update" << endl;
    //     window.update();
    // }

    return 0;
}

Server::Server() : Streaming("Server"){}