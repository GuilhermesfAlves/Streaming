#include "headers/streaming.hpp"

Streaming::Streaming(string log_name): socket(new MySocket(log_name)){}

void Streaming::toConnect(char* connection){
    int sockfd = socket -> getSockfd();
    struct packet_mreq mr = {0};
    int ifindex;

    ifindex = if_nametoindex(connection);

    // Inicializa socket
    socket -> toBind(ifindex);

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