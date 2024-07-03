#include "headers/client.hpp"
#include "headers/server.hpp"

using namespace std;

Streaming* streamingFactory(char* mode){
    if (!strcasecmp(mode, SERVER_SOCKET_STR)){
        return new Server();
    } else if (!strcasecmp(mode, CLIENT_SOCKET_STR))
        return new Client();
    return nullptr;
}

bool isValidPort(int port){
    return ((port > 1024) && (port < 65535));
}

int main(int argc, char* argv[]) {
    string executeTemplate = "Use: ./exec <mode = client | server> <conection port, Ex: 12345>"; 
    int port;
    
    // Verifica se há exatamente 3 argumentos
    if (argc != 3) {
        cerr << executeTemplate << endl;
        exit(EXIT_FAILURE);
    }

    if (!isValidPort(port = stoi(argv[2]))){
        cerr << "invalid port, try one at the interval: [1024..65535]" << endl;
        exit(EXIT_FAILURE);
    }
    Streaming* streaming;
    if (!(streaming = streamingFactory(argv[1]))){
        cerr << "undefined mode, try <client> or <server>" << endl;
        exit(EXIT_FAILURE);
    }

    streaming -> toConnect(port);
    streaming -> handshake();
    streaming -> run();
    return 0;
}