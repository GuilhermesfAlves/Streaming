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

bool isValidConnection(char* connection){
    return (!(strcmp("lo", connection)) || !(strcmp("enp3s0", connection)));
}

int main(int argc, char* argv[]) {
    string executeTemplate = "Use: ./exec [<client> | <server>] [<lo> <enp3s0>]"; 
    
    // Verifica se há exatamente 3 argumentos
    if ((argc != 3) || (!isValidConnection(argv[2]))) {
        cerr << executeTemplate << endl;
        exit(EXIT_FAILURE);
    }

    Streaming* streaming;
    if (!(streaming = streamingFactory(argv[1]))){
        cerr << "undefined mode, try <client> or <server>" << endl;
        exit(EXIT_FAILURE);
    }

    streaming -> toConnect(argv[2]);
    streaming -> run();
    return 0;
}