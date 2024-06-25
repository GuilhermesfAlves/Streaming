#include "../headers/server.hpp"

int serverMethod(int port){
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Erro ao abrir o socket" << endl;
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Erro ao associar o socket" << endl;
        close(sockfd);
        return -1;
    }

    listen(sockfd, 5);
    cout << "Servidor aguardando conexões na porta " << port << endl;

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        cerr << "Erro ao aceitar conexão" << endl;
        close(sockfd);
        return -1;
    }

    cout << "Servidor aceitou uma conexão" << endl;
    close(newsockfd);
    close(sockfd);

    return 0;
}