#include <iostream>
#include <cstring>  // para usar strcmp
#include "../headers/client.hpp"
#include "../headers/server.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    unsigned char mode;
    int port;
    
    // Verifica se há exatamente 3 argumentos
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <modo = client | server> <porta de conexao, Ex: 12345>" << endl;
        return -1;
    }

    port = stoi(argv[2]);

    // Define as macros CLIENT ou SERVER dependendo do argumento 'modo'
    if (!strcmp(argv[1], "client")) {
        mode = CLIENT_MODE;
    } else if (!strcmp(argv[1], "server")) {
        mode = SERVER_MODE;
    } else {
        cerr << "Modo desconhecido: " << argv[1] << endl;
        return -1;
    }

    // Exemplo de uso das macros definidas
    if (mode == CLIENT_MODE){
        clientMethod("127.0.0.1",port);
    } else if (mode == SERVER_MODE){
        serverMethod(port);
    }

    return 0;
}
