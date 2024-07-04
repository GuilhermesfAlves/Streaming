#include "headers/client.hpp"
#include <cstdlib> // Para rand() e srand()
#include <ctime> // Para time()

#define MAX_PWD 100000
#define MIN_PWD 1000

int Client::run(){
    char buffer[BUFFER_SIZE] = {0};
    Message m;

    m.deserializeMessage(T_PRINT, "Hello, TCP Server!");
    socket -> post(m.getMessage());

    socket -> collect(buffer);
    m.setMessage(buffer);
    cout << "Mensagem recebida: " << m.getData() << endl;
    return 0;
}

Client::Client() : Streaming("Client"){}