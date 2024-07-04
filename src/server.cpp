#include "headers/server.hpp"


int Server::run(){
    char buffer[BUFFER_SIZE];
    Message m;
    socket -> collect(buffer);
    m.setMessage(buffer);
    cout << "Mensagem recebida: " << m.getData() << endl;

    m.deserializeMessage(T_PRINT, "Hello from server!");
    socket -> post(m.getMessage());

    return 0;
}

void Server::handshake(){
    struct sockaddr_in clientAddress;
    socklen_t len = sizeof(clientAddress);
    char buffer[BUFFER_SIZE] = {0};
    Message m;
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
    m.setMessage(buffer);
    int pwd = atoi(m.getData());
    if (pwd == 0){
        memset(buffer, 0, BUFFER_SIZE);
        socket -> post(buffer);
        socket -> logger -> output("Handshake Failed 1");
        exit(EXIT_FAILURE);
    }
    sprintf (buffer, "%d", ++pwd);
    m.deserializeMessage(T_PRINT, buffer);
    socket -> post(m.getMessage());
    socket -> collect(buffer);
    m.setMessage(buffer);
    if (m.getType() != T_ACK){
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
