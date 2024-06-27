#ifndef __UTILS__
#define __UTILS__

#include <sys/socket.h>
#include <iostream>
#include <cstring>  // para usar strcmp
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>

#include "socket.hpp"
using namespace std;


#define CLIENT_SOCKET_STR "Client"
#define SERVER_SOCKET_STR "Server"
#define START_MARK 126
#define BUFFER_SIZE 64

int createSocket();
int createServerConnection(int sockfd, int port);
int createClientConnection(int sockfd, int port);

#endif