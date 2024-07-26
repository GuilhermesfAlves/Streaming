#include "include/client.hpp"

Client::Client() : Streaming(CLIENT_SOCKET_STR){}

int Client::run(){

    string path;
    do {
        getUserAction();
        switch (action){
        case T_LIST:
            // request para server de quais as opções do catalogo
            // print na tela quais as opções T_LIST
            
            single.send(T_LIST);
            // single.receive(SHORT_TIMEOUT);
            // fileCount = single.getDataNumber();
            // while (window.dataSize() != fileCount){
            cout << "In cathalog:" << endl;
            window.receive(SHORTEST_TIMEOUT, 1);
            // }
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
            fileSize = single.getDataNumber();
            path = CLIENT_CATHALOG_FOLDER;
            path.append(videoToDownload);
            free(videoToDownload);
            {
                int fileStatus;
                string command;
                if ((fileStatus = window.tryBuildDataFile(path.c_str(), fileSize)) != FILE_OPEN_SUCCESS)
                    single.send(T_ERROR, fileStatus);
                else {
                    window.receive(SHORTEST_TIMEOUT, fileSize);
                    command = "./vlc-wrapper.sh " + path;
                    system(command.c_str());
                }
            }
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
    char str[MAX_DATA_SIZE];
    cout << "Select a video" << endl << ">";
    cin >> str;
    videoToDownload = strdup(str);
    action = T_DOWNLOAD;
}
