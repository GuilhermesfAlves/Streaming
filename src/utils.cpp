#include "headers/utils.hpp"

#ifdef __PRD_MODE__

int createSocket(int* sockfd){
    return socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
}

int createServerConnection(int* sockfd, int port){
    struct sockaddr_ll endereco = {0};
    struct packet_mreq mr = {0};
    int ifindex;

    ifindex = if_nametoindex("enp3s0");

    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(*sockfd, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
        cerr << "Erro ao fazer bind no socket" << endl;
        return 0;
    }

    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(*sockfd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        cerr << "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente." << endl;
        close(*sockfd);
        return 0;
    }

    // The IPv4 layer generates an IP header when sending a packet,
    // unless the IP_HDRINCL socket option is enabled on the socket.
    // const int on = 1;
    // if (setsockopt (*sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof (on)) < 0) {
    //     cerr << "setsockopt() failed to set IP_HDRINCL " << endl;
    //     return 0;
    // }

    return 1;
}

int createClientConnection(){
    return 1;
}

#endif

#ifdef __DEV_MODE__

int createSocket(){
    return socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
}

int createServerConnection(int sockfd, int port){
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int addrlen = sizeof(serverAddress);
    char buffer[BUFFER_SIZE];

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Inicializa socket
    if (bind(sockfd, (struct sockaddr*) &serverAddress, (socklen_t)addrlen) < 0) {
        cerr << "Erro ao fazer bind no socket" << endl;
        close(sockfd);
        return 0;
    }

    memset(&clientAddress, 0, sizeof(clientAddress));
    socklen_t len = sizeof(clientAddress);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &len);
    buffer[n] = '\0';
    cout << "Cliente: " << buffer << endl;

    // Conecta o socket ao endereço do cliente para comunicação simplificada
    if (connect(sockfd, (struct sockaddr *)&clientAddress, len) < 0) {
        cerr << "Erro ao conectar ao cliente" << endl;
        close(sockfd);
        return 0;
    }
    return 1;
}

int createClientConnection(int sockfd, int port){
    const char *message = "Hello from client";
    struct sockaddr_in servaddr;

    memset(&servaddr, 0, sizeof(servaddr));

    // Configura o endereço do servidor
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conecta o socket ao endereço do servidor
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        cerr << "Erro ao conectar ao servidor" << endl;
        close(sockfd);
        return 0;
    }

    // Envia a mensagem para o servidor usando send
    if (send(sockfd, message, strlen(message), 0) < 0){
        cout << "Mensagem de inicio de conexão não enviada " << endl;
    } else {
        cout << "Mensagem de inicio de conexão enviada" << endl;
    }

    return 1;
}

#endif