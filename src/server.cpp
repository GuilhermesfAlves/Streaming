#include "../headers/server.hpp"


int serverMethod(int port) {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];
    const char *response = "Hello from TCP Server!";

    // Cria um socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Erro ao criar socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Configura o endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // O servidor escutará em qualquer IP disponível
    server_addr.sin_port = htons(port);

    // Associa o socket ao endereço e porta
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Erro ao associar socket" << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Coloca o socket em modo de escuta
    if (listen(sockfd, 5) < 0) {
        cerr << "Erro ao colocar o socket em escuta" << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    cout << "Servidor em escuta na porta " << port << endl;

    client_len = sizeof(client_addr);

    // Aceita uma conexão de um cliente
    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (newsockfd < 0) {
        cerr << "Erro ao aceitar conexão" << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    cout << "Cliente conectado" << endl;

    // Recebe a mensagem do cliente
    int len = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        buffer[len] = '\0';
        cout << "Mensagem recebida: " << buffer << endl;

        // Envia uma resposta ao cliente
        if (send(newsockfd, response, strlen(response), 0) < 0) {
            cerr << "Erro ao enviar resposta" << endl;
            close(newsockfd);
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        cout << "Resposta enviada ao cliente" << endl;
    } else {
        cerr << "Erro ao receber mensagem" << endl;
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}