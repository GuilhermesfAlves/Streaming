#include "include/client.hpp"


int Client::run(){

    do {
        cout << "client - run - do" << endl;
        getUserAction();
        switch (action){
        case T_LIST:
            // request para server de quais as opções do catalogo
            // print na tela quais as opções T_LIST
            
            single.send(T_LIST);
            single.receive(LONG_TIMEOUT);
            fileCount = single.getDataNumber();
            while (window.collectedSize() != fileCount)
                window.getWindow();
            cout << "In cathalog:" << endl;
            window.printCollected();
            window.flushCollected();
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
            if (single.receive(DEFAULT_TIMEOUT) == T_ERROR){
                cout << "Invalid file, try again" << endl;
                action = T_LIST;
                break;
            } 
            fileCount = single.getDataNumber() / MAX_DATA_SIZE;
            while (window.collectedSize() != fileCount)
                window.getWindow();

            if (window.buildCollectedFile(videoToDownload) == FILE_FULL_DISK)
                single.send(T_ERROR, ERROR_FULL_DISK);

            window.flushCollected();
            break;
        default:
            break;
        }
    } while(action != T_END_TX);
    single.send(T_END_TX);

    // window.add(T_PRINT, "012345678901234567890123456789012345678901234567890123456789012");
    // window.add(T_PRINT, "2 Second message! :)");
    // window.add(T_PRINT, "3 Third message! :)");
    // window.add(T_PRINT, "4 Fourth message :)");
    // window.add(T_PRINT, "5 Fifth message! :)");

    // while (!window.empty()){
    //     cout << "update" << endl;
    //     window.update();
    // }

    // cout << "fim envio, comeco collect" << endl;
    // while (window.collectedSize() != 1){
    //     cout << "collect" << endl;
    //     window.getWindow();
    // }

    return 0;
}

int Client::getUserAction(){

    if (action == T_INEXISTENT){
        return menuAction();
    } else if (action == T_LIST){
        return cathalogAction();
    } else return 0; //MUDAR
}

int Client::menuAction(){
    char entry;
    cout << "Hello user!" << endl;
    while (1){
        cout << "To list the cathalog, type <L> or <l>" << endl;
        cout << "To exit, type <E> or <e>" << endl;
        cin >> entry;
        switch (entry){
        case 'L':
        case 'l':
            return (action = T_LIST);
        case 'E':
        case 'e':
            return (action = T_END_TX);
        default:
            cout << "Type a valid option" << endl;
            break;
        }
    }
    return (action = T_INEXISTENT);
}

int Client::cathalogAction(){
    
    cout << "Select a video" << endl;
    cin >> videoToDownload;
    return T_DOWNLOAD;
}


Client::Client() : Streaming("Client"){}