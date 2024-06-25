#include "../headers/client.hpp"

int clientMethod(int port){
    // Lógica do Cliente
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Erro ao abrir o socket" << endl;
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Erro ao conectar" << endl;
        close(sockfd);
        return -1;
    }

    cout << "Cliente conectado ao servidor na porta " << port << endl;
    close(sockfd);
    return 0;
}