#include "headers/socket.hpp"
#define TX "Tx"
#define RX "Rx"

MySocket::MySocket(string socketType) : sockfd(createSocket()), logger(new Logger(socketType)){
    lastSent = (char*)calloc(MAX_MESSAGE_SIZE, 1);
}

MySocket::~MySocket(){
    close(this -> sockfd);
    free(lastSent);
}

void MySocket::post(char* buffer){
    write(this -> sockfd, buffer, BUFFER_SIZE);
    logger -> log(buffer, TX);
    strcpy(lastSent,buffer);
}

char* MySocket::collect(char* buffer){
    do {
        memset(buffer, 0, BUFFER_SIZE);
        read(this -> sockfd, buffer, BUFFER_SIZE);
    } while ((!strcmp(lastSent, buffer)) || (buffer[0] != HEAD_MARK));
    strcpy(lastSent, buffer);
    logger -> log(buffer, RX);
    return buffer;
}

void MySocket::toBind(int ifindex){
    sockaddr_ll endereco;

    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(sockfd, (struct sockaddr*) &endereco, sizeof(endereco)) < 0) {
        logger -> output("Error: Can't Bind");
        exit(EXIT_FAILURE);
    }
}

void MySocket::setSocketOpt(int ifindex){
    struct packet_mreq mr = {0};

    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(sockfd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        cerr << "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente." << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

int MySocket::createSocket(){
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
        logger -> output("Build socket error");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}