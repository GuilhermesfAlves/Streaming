#ifndef CLIENT
#define CLIENT

    #include "streaming.hpp"

    class Client: public Streaming{
    private:
        int videoToDownload;
        int getUserAction();
        int menuAction();
        int cathalogAction();
    public:
        Client();
        int run() override;
    };

#endif