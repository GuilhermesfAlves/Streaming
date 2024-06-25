#include "../headers/client.hpp"

int clientMethod(const char* server_ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    const char *message = "Hello, TCP Server!";

    // Cria um socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        cerr << "Erro ao criar socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Configura o endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        cerr << "Endereço inválido/Não suportado" << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Conecta ao servidor
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Erro ao conectar" << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Envia a mensagem
    if (send(sockfd, message, strlen(message), 0) < 0) {
        cerr << "Erro ao enviar mensagem" << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    cout << "Mensagem enviada ao servidor" << endl;

    // Recebe a resposta do servidor
    int len = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        buffer[len] = '\0';
        cout << "Resposta do servidor: " << buffer << endl;
    } else if (len == 0) {
        cout << "Conexão fechada pelo servidor" << endl;
    } else {
        cerr << "Erro ao receber resposta" << endl;
    }

    close(sockfd);
    return 0;
}