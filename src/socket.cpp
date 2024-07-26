#include "include/socket.hpp"
#define TX "Tx"
#define RX "Rx"

Socket* Socket::instance = NULL;

Socket::Socket(string socketType){
    if (socketType.empty())
        return;
        
    sockfd = createSocket();
    logger = new Logger(socketType);
}

Socket* Socket::instanceOf(string socketType){
    return (instance)? instance : (instance = new Socket(socketType));
}

Socket::~Socket(){
    close(sockfd);
}

void Socket::post(const void* buffer, size_t len){
    int bytes = -1;
    if (len < 14)   
        len = 14;
    logger -> log((char*)buffer, TX);
    while (bytes == -1)
        bytes = write(sockfd, buffer, len);
}

char* Socket::collect(char* buffer){
    int len;

    memset(buffer, 0, BUFFER_SIZE);
    len = read(sockfd, buffer, BUFFER_SIZE);
    //ouviu algo 
    if (len > 0)
        logger -> log(buffer, RX);
    else
        memset(buffer, 0, BUFFER_SIZE);
    return buffer;
}

void Socket::toBind(int ifindex){
    sockaddr_ll endereco;

    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(sockfd, (struct sockaddr*) &endereco, sizeof(endereco)) < 0) {
        cerr << "Error: Can't Bind" << endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::setSocketPromisc(int ifindex){
    struct packet_mreq mr = {0};

    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(sockfd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) < 0) {
        cerr << "Erro ao definir PACKET_MR_PROMISC"
            "Verifique se a interface de rede foi especificada corretamente." << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void Socket::setSocketTimeout(int timeoutMillis){
    struct timeval timeout;

    timeout.tv_sec = timeoutMillis / 1000;
    timeout.tv_usec = (timeoutMillis % 1000) * 1000;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout)) == -1) {
        cerr << "Erro ao definir Timeout: "
            "Verifique se timeoutMillis foi especificada corretamente." << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*) &timeout, sizeof(timeout)) == -1) {
        cerr << "Erro ao definir Timeout: "
            "Verifique se timeoutMillis foi especificada corretamente." << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

int Socket::createSocket(){
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
        cerr << "Build socket error" << endl;
        exit(EXIT_FAILURE);
    }
    return sockfd;
}
