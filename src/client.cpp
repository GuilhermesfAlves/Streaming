#include "include/client.hpp"
#include <limits>

Client::Client() : Streaming(CLIENT_SOCKET_STR){}

int Client::run(){

    string path;
    tm time;
    do {
        getUserAction();
        switch (action){
        case T_LIST:
            // request para server de quais as opções do catalogo
            // print na tela quais as opções T_LIST
            
            single.send(T_LIST);
            cout << "In cathalog:" << endl;
            while (single.receive(SHORT_TIMEOUT) != T_END_TX);
            break;
        case T_DOWNLOAD:
            // função para escolha de arquivo para baixar
            // request para server T_DOWNLOAD
            //  recebe o tamanho do arquivo T_FILE_DESCRIPTOR
            //  recebe os dados do arquivo T_DATA
            //  enquanto isso pode permitir ao usuario cancelar o download
            //  TODO recebe a request de abrir um player
            single.send(T_DOWNLOAD, videoToDownload.c_str());
            if (!strcasecmp(videoToDownload.c_str(), "quit")){
                action = T_INEXISTENT;
                break;
            }
            //possibilidade de receber uma mensagem de ERROR_FILE_NOT_FOUND
            if (single.receive(SHORTEST_TIMEOUT) == T_ERROR){
                cout << "Invalid file, try again" << endl;
                action = T_LIST;
                break;
            } 
            fileSize = single.getFileSize();
            time = single.getFileData();
            {
                int fileStatus;
                string command;
                try{
                    if ((fileStatus = window.tryBuildDataFile(CLIENT_CATHALOG_FOLDER, videoToDownload, fileSize, &time)) != FILE_OPEN_SUCCESS){
                        single.send(T_ERROR, fileStatus);
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (fileStatus == FILE_OPEN_FAIL)
                            cout << "Can't open file, try again later" << endl;
                        else 
                            cout << "Not enough space in disk" << endl;
                        cout << "Press enter to continue" << endl;
                        cin.get();
                    }
                    else {
                        window.receive(SHORTEST_TIMEOUT, fileSize);

                        command = "./vlc-wrapper.sh " + (string)CLIENT_CATHALOG_FOLDER + videoToDownload;
                        system(command.c_str());
                        ostringstream oss;
                        oss << put_time(&time, "%Y%m%d%H%M");
                        command = "touch -t " + oss.str() + (string)CLIENT_CATHALOG_FOLDER + videoToDownload;
                        system(command.c_str());
                    }
                } catch(exception &e){
                    single.send(T_ERROR, 3);
                    command = "./vlc-wrapper.sh " + (string)CLIENT_CATHALOG_FOLDER + videoToDownload;
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

    window.flushHistoric();

    if (action == T_INEXISTENT){
        menuAction();
    } else if (action == T_LIST){
        cathalogAction();
    } 
}

void Client::menuAction(){
    char entry;
    system("clear");
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
    cout << "Select a video or type 'quit' to go back" << endl << ">";
    cin >> str;
    videoToDownload = strdup(str);
    action = T_DOWNLOAD;
}
