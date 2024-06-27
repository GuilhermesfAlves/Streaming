#include "headers/server.hpp"



// // Estrutura da mensagem customizada
// struct CustomMessage {
//     uint16_t messageSize;  // Tamanho da mensagem em bytes
//     uint16_t messageId;    // ID da mensagem
//     uint8_t crcType;       // Tipo de CRC (por exemplo, CRC8, CRC16)
//     // Aqui você pode adicionar outros campos da sua mensagem
//     char data[512];        // Dados da mensagem
// };

Server::Server() : Streaming("Server"){}
#ifdef __DEV_MODE__

void Server::toConnect(int port){
    int sockfd = socket -> getSockfd();
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }
}
#endif

#ifdef __PRD_MODE__

void Server::toConnect(int port){
    int sockfd = socket -> getSockfd();
    struct sockaddr_ll endereco = {0};
    struct packet_mreq mr = {0};
    int ifindex;

    ifindex = if_nametoindex("enp3s0");

    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(sockfd, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
        cerr << "Erro ao fazer bind no socket" << endl;
        exit(EXIT_FAILURE);
    }

    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(sockfd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        cerr << "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente." << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // The IPv4 layer generates an IP header when sending a packet,
    // unless the IP_HDRINCL socket option is enabled on the socket.
    // const int on = 1;
    // if (setsockopt (*sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof (on)) < 0) {
    //     cerr << "setsockopt() failed to set IP_HDRINCL " << endl;
    //     return 0;
    // }

}
#endif

int Server::run(){
    char buffer[BUFFER_SIZE];

    socket -> collect(buffer);
    std::cout << "Mensagem recebida: "<< buffer << endl;

    std::strcpy(buffer,"Hello from server!");
    socket -> post(buffer);

    return 0;
}