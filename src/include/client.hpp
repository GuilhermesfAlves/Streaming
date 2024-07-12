#ifndef CLIENT
#define CLIENT

    #include "streaming.hpp"

    class Client: public Streaming{
    private:
        char videoToDownload[MAX_MESSAGE_SIZE];
        int getUserAction();
        int menuAction();
        int cathalogAction();
    public:
        Client();
        int run() override;
    };

#endif