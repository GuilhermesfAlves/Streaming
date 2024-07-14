#include "include/server.hpp"
#include <filesystem>

int Server::run(){

    char* path;
    char* fileName;
    do {
        action = single.receive(INFINIT_TIMEOUT);
        switch (action){
        case T_LIST:
            //ao receber o pedido de listagem de arquivos,
            //retorna a listagem com N aquivos do tipo T_LIST de novo
            //e manda em janela os nomes dos N arquivos do tipo T_FILE_DESCRIPTOR 
            getFilesInCathalogCount();
            single.send(T_LIST, fileCount);
            getFilesInCathalogToWindow();
            window.send(LONG_TIMEOUT);
            break;
        case T_DOWNLOAD:
            //confere se o arquivo referenciado na mensagem existe,
            // se sim, envia numa mensagem unica o tamanho como T_FILE_DESCRIPTOR, em seguida em janela os dados como T_DATA
            // caso contrário, envia T_ERROR com o numero 2, como não encontrado
            
            //caso no meio da transmissão dos dados o cliente resolva parar de receber ou
            //envie o erro de T_ERROR 3, de disco cheio, deve-se parar a transmissão
            fileName = single.getDataStr();
            path = new char[strlen(SERVER_CATHALOG_FOLDER) + strlen(fileName)];
            strcpy(path, SERVER_CATHALOG_FOLDER);
            strcat(path, fileName);
            if (!(file = new ifstream(path, ios::binary | ios::ate | ios::in)) || (!file -> is_open())){
                single.send(T_ERROR, ERROR_FILE_NOT_FOUND);
                break;
            }
            single.send(T_FILE_DESCRIPTOR, file -> tellg());
            file -> seekg(0, ios::beg);
            window.add(T_DATA, file);
            window.send(LONG_TIMEOUT);
            file -> close();
            break;
        case T_ERROR:
            break;
        default:
            break;
        }
    } while (action != T_END_TX);
    
    return 0;
}

Server::Server() : Streaming(SERVER_SOCKET_STR, SERVER_MODE){}

void Server::getFilesInCathalogCount(){
    try {
        for (const auto& entry : filesystem::directory_iterator(SERVER_CATHALOG_FOLDER)) {
            if (entry.is_regular_file()) {
                ++fileCount;
            } 
        }
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Erro: " << e.what() << std::endl;
    }
}

void Server::getFilesInCathalogToWindow(){
    try {
        for (const auto& entry : filesystem::directory_iterator(SERVER_CATHALOG_FOLDER)) {
            if (entry.is_regular_file()) {
                window.add(T_FILE_DESCRIPTOR, entry.path().filename().string().c_str());
            } 
        }
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Erro: " << e.what() << std::endl;
    }
}
