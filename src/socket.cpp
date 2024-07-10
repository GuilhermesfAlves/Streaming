#include "include/socket.hpp"
#define TX "Tx"
#define RX "Rx"

MySocket::MySocket(string socketType) : sockfd(createSocket()), logger(new Logger(socketType)){
    lastSent = (char*)calloc(MAX_MESSAGE_SIZE, 1);
}

MySocket::~MySocket(){
    close(sockfd);
    free(lastSent);
}

void MySocket::post(const void* buffer, size_t len){
    // cout << "posting message len: " << len << endl;
    // cout << "buffer: " << (char*)buffer << endl;
    if (len < 14)   
        len = 14;
    int bytes = -1;
    logger -> log((char*)buffer, TX);
    while (bytes == -1)
        bytes = write(sockfd, buffer, len);
    // cout << "bytes sent: " << bytes << endl;
    strcpy(lastSent, (char*)buffer);
}

char* MySocket::collect(char* buffer){
    int len;

    cout << "trying" << endl;
    memset(buffer, 0, BUFFER_SIZE);
    len = read(sockfd, buffer, BUFFER_SIZE);
    //ouviu nada       // escutou a si mesmo
    if ((len <= 0) || (!strcmp(lastSent, buffer))){
        memset(buffer, 0, BUFFER_SIZE);
        return buffer;
    }

    cout << "coletado len: " << len << " buffer: " << buffer << endl;
    cout << "last Sent" << lastSent << endl;
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

void MySocket::setSocketPromisc(int ifindex){
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
    int debug = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_DEBUG, &debug, sizeof(debug)) < 0) {
        perror("Erro ao definir SO_DEBUG");
        close(sockfd);
    }
}

void MySocket::setSocketTimeout(int timeoutMillis){
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

int MySocket::createSocket(){
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
        logger -> output("Build socket error");
        exit(EXIT_FAILURE);
    }
    cout << "sock nº: "<< sockfd << endl;   
    return sockfd;
}
