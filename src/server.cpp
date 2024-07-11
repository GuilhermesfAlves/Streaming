#include "include/server.hpp"
#include <filesystem>


int Server::run(){

    do {
        action = single.receive();
        switch (action){
        case T_LIST:
            //ao receber o pedido de listagem de arquivos,
            //retorna a listagem com N aquivos do tipo T_LIST de novo
            //e manda em janela os nomes dos N arquivos do tipo T_FILE_DESCRIPTOR 
            getFilesInCathalog();
            while (!single.send(T_LIST, fileCount));
            while (!window.empty())
                window.update();
            break;
        case T_DOWNLOAD:
            //confere se o arquivo referenciado na mensagem existe,
            // se sim, envia numa mensagem unica o tamanho como T_FILE_DESCRIPTOR, em seguida em janela os dados como T_DATA
            // caso contrário, envia T_ERROR com o numero 2, como não encontrado
            
            //caso no meio da transmissão dos dados o cliente resolva parar de receber ou
            //envie o erro de T_ERROR 3, de disco cheio, deve-se parar a transmissão

            file -> open("example.txt", ios::binary);
            if (!file -> is_open()){
                //error file dont exists
            }
            //while(!single.send(T_FILE_DESCRIPTOR, sizeof(file)))
            window.add(T_DATA, file);
            while(!window.empty())
                window.update();

            break;
        case T_ERROR:
            break;
        default:
            break;
        }
    } while (action != T_END_TX);
    

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

void Server::getFilesInCathalog(string path){
    try {
        for (const auto& entry : filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                ++fileCount;
                window.add(T_FILE_DESCRIPTOR, entry.path().filename().string().c_str());
            } else if (entry.is_directory()) {
                cout << entry.path().filename().string() << endl;
                getFilesInCathalog(path.append("/").append(entry.path().filename().string()));
            }
        }
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Erro: " << e.what() << std::endl;
    }
}

void Server::getFilesInCathalog(){
    getFilesInCathalog(path);
}