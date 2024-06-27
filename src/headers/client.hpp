#ifndef CLIENT
#define CLIENT

    #include "streaming.hpp"

    class Client: public Streaming{
    public:
        Client();

        int run() override;
        void handshake() override;
        void toConnect(int port) override;
    };

#endif