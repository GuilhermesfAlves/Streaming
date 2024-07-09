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
#include <sys/time.h>
#include "message.hpp"

using namespace std;

#define DEFAULT_TIMEOUT 1000
#define CLIENT_SOCKET_STR "Client"
#define SERVER_SOCKET_STR "Server"
#define CLIENT_MODE 0
#define SERVER_MODE 1
#define BUFFER_SIZE MAX_MESSAGE_SIZE

class Logger{
private:
    ofstream logFile;
public:
    Logger(string fileName);
    ~Logger();
    void log(const char* message, const char* from);
    void output(const char* message);
};

#endif