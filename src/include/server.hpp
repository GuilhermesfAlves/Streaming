#ifndef SERVER
#define SERVER

#define SERVER_CATHALOG_FOLDER "cathalogS/"

#include "streaming.hpp"

class Server: public Streaming{
private:
    ifstream* file;
    void sendFilesInCathalog();
public:
    Server();

    int run() override;
};

#endif