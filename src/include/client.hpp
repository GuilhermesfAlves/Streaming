#ifndef CLIENT
#define CLIENT

#define CLIENT_CATHALOG_FOLDER "cathalogC/"

#include "streaming.hpp"

class Client: public Streaming{
private:
    int fileSize;
    string videoToDownload = "";
    void getUserAction();
    void menuAction();
    void cathalogAction();
public:
    Client();
    int run() override;
};

#endif