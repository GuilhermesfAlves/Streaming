#include "headers/socket.hpp"
#define TX "Tx"
#define RX "Rx"

MySocket::MySocket(string socketType) : sockfd(createSocket()), logger(new Logger(socketType)){
    lastSent = (char*)calloc(MAX_MESSAGE_SIZE, 1);
}

MySocket::~MySocket(){
    close(sockfd);
    free(lastSent);
}

void MySocket::post(char* buffer){
    logger -> log(buffer, TX);
    write(sockfd, buffer, strlen(buffer));
    strcpy(lastSent, buffer);
}

char* MySocket::collect(char* buffer){
    int counter = 0;
    int len;
    cout << "," << endl;
    do {
        cout << "trying" << endl;
        memset(buffer, 0, BUFFER_SIZE);
        len = read(sockfd, buffer, BUFFER_SIZE);
        counter++;
        //  até 4 tentativas  //ouviu nada   // escutou a si mesmo          // mensagem sniffada
    } while ((counter < 4) && ((len < 0) || (!strcmp(lastSent, buffer)) || (buffer[0] != HEAD_MARK)));
    cout << "coletado len: "<< len << " buffer: " << buffer << endl;
    if (!strlen(buffer))
        return buffer;
    strcpy(lastSent, buffer);
    logger -> log(buffer, RX);
    cout << "." << endl;
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
    if (setsockopt(sockfd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        cerr << "Erro ao definir PACKET_MR_PROMISC"
            "Verifique se a interface de rede foi especificada corretamente." << endl;
        close(sockfd);
        exit(EXIT_FAILURE);
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
}

int MySocket::createSocket(){
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
        logger -> output("Build socket error");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}