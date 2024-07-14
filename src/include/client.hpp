#ifndef CLIENT
#define CLIENT

#define CLIENT_CATHALOG_FOLDER "cathalogC/"

#include "streaming.hpp"

class Client: public Streaming{
private:
    char videoToDownload[MAX_MESSAGE_SIZE];
    void getUserAction();
    void menuAction();
    void cathalogAction();
public:
    Client();
    int run() override;
};

#endif