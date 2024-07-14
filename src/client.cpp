#include "include/client.hpp"


int Client::run(){

    char* path;
    do {
        getUserAction();
        switch (action){
        case T_LIST:
            // request para server de quais as opções do catalogo
            // print na tela quais as opções T_LIST
            
            single.send(T_LIST);
            single.receive(LONG_TIMEOUT);
            fileCount = single.getDataNumber();
            while (window.dataSize() != fileCount){
                window.receive(SHORTEST_TIMEOUT);
            }
            cout << "In cathalog:" << endl;
            window.printData();
            window.flushData();

            break;
        case T_DOWNLOAD:
            // função para escolha de arquivo para baixar
            // request para server T_DOWNLOAD
            //  recebe o tamanho do arquivo T_FILE_DESCRIPTOR
            //  recebe os dados do arquivo T_DATA
            //  enquanto isso pode permitir ao usuario cancelar o download
            //  TODO recebe a request de abrir um player
            single.send(T_DOWNLOAD, videoToDownload);
            //possibilidade de receber uma mensagem de ERROR_FILE_NOT_FOUND
            if (single.receive(SHORTEST_TIMEOUT) == T_ERROR){
                cout << "Invalid file, try again" << endl;
                action = T_LIST;
                break;
            } 
            fileCount = (single.getDataNumber() / MAX_DATA_SIZE) + 1;
            //funcionando até aqui
            while (window.dataSize() != fileCount)
                window.receive(DEFAULT_TIMEOUT);

            path = new char[strlen(CLIENT_CATHALOG_FOLDER) + strlen(videoToDownload)];
            strcpy(path, CLIENT_CATHALOG_FOLDER);
            strcat(path, videoToDownload);
            if (window.buildDataFile(path) == FILE_FULL_DISK)
                single.send(T_ERROR, ERROR_FULL_DISK);
            delete path;
            window.flushData();
            action = T_INEXISTENT;
            break;
        default:
            break;
        }
    } while(action != T_END_TX);
    single.send(T_END_TX);
    return 0;
}

void Client::getUserAction(){

    if (action == T_INEXISTENT){
        menuAction();
    } else if (action == T_LIST){
        cathalogAction();
    } else ; //MUDAR
}

void Client::menuAction(){
    char entry;
    cout << "Hello user!" << endl;
    while (1){
        cout << "To list the cathalog, type <L> or <l>" << endl;
        cout << "To exit, type <E> or <e>" << endl;
        cin >> entry;
        switch (entry){
        case 'L':
        case 'l':
            action = T_LIST;
            return;
        case 'E':
        case 'e':
            action = T_END_TX;
            return;
        default:
            cout << "Type a valid option" << endl;
            break;
        }
    }
}

void Client::cathalogAction(){
    
    cout << "Select a video" << endl << ">";
    cin >> videoToDownload;
    action = T_DOWNLOAD;
}


Client::Client() : Streaming(CLIENT_SOCKET_STR, CLIENT_MODE){}
