#ifndef SERVER
#define SERVER

    #include "streaming.hpp"

    class Server: public Streaming{
    public:
        Server();

        void toConnect(int port) override;
        int run() override;
    };

#endif