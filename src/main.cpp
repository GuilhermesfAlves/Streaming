#include "include/client.hpp"
#include "include/server.hpp"

using namespace std;

Streaming* streamingFactory(char* mode){
    if (!strcasecmp(mode, SERVER_SOCKET_STR)){
        return new Server();
    } else if (!strcasecmp(mode, CLIENT_SOCKET_STR))
        return new Client();
    return NULL;
}

int main(int argc, char* argv[]) {
    string executeTemplate = "Use: sudo ./exec [<client> | <server>] [your web interface: try running ifconfig to see which are avaiable]"; 
    
    // Verifica se há exatamente 3 argumentos
    if (argc != 3) {
        cerr << executeTemplate << endl;
        exit(EXIT_FAILURE);
    }

    Streaming* streaming;
    if (!(streaming = streamingFactory(argv[1]))){
        cerr << "undefined mode, try <client> or <server>" << endl;
        exit(EXIT_FAILURE);
    }
    streaming -> toConnect(argv[2]);
    try{
        streaming -> run();
        return 0;
    } catch (const exception& e){
        cout << endl;
        cout << e.what() << endl;
        cout << "Finishing Execution" << endl;
        return 1;
    }
}
