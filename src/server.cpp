#include "headers/server.hpp"



// // Estrutura da mensagem customizada
// struct CustomMessage {
//     uint16_t messageSize;  // Tamanho da mensagem em bytes
//     uint16_t messageId;    // ID da mensagem
//     uint8_t crcType;       // Tipo de CRC (por exemplo, CRC8, CRC16)
//     // Aqui você pode adicionar outros campos da sua mensagem
//     char data[512];        // Dados da mensagem
// };

int Server::run(){
    char buffer[BUFFER_SIZE];

    socket -> collect(buffer);
    cout << "Mensagem recebida: "<< buffer << endl;

    strcpy(buffer,"Hello from server!");
    socket -> post(buffer);

    return 0;
}

void Server::handshake(){
    struct sockaddr_in clientAddress;
    socklen_t len = sizeof(clientAddress);
    char buffer[BUFFER_SIZE] = {0};
    // Essa parte teria que mudar para fazer em RAW 
    // ============================================
    memset(&clientAddress, 0, sizeof(clientAddress));
    if (recvfrom(socket -> getSockfd(), buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &len) < 0){
        cout << "Failed to recieve Handshake: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    // Conecta o socket ao endereço do cliente para comunicação simplificada
    socket -> createConnection(&clientAddress);
    // ============================================
    int pwd = atoi(buffer);
    if (pwd == 0){
        memset(buffer, 0, BUFFER_SIZE);
        socket -> post(buffer);
        socket -> logger -> output("Handshake Failed 1");
        exit(EXIT_FAILURE);
    }
    sprintf (buffer, "%d", ++pwd);
    socket -> post(buffer);
    socket -> collect(buffer);
    if (strcmp(buffer, PURE_ACK)){
        socket -> logger -> output("Handshake Failed 2");
        exit(EXIT_FAILURE);
    }
}

Server::Server() : Streaming("Server"){}
#ifdef __DEV_MODE__

void Server::toConnect(int port){
    struct sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Inicializa socket
    socket -> toBind(&serverAddress);
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
