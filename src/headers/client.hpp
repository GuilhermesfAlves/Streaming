#ifndef CLIENT
#define CLIENT

    #include "streaming.hpp"

    class Client: public Streaming{
    public:
        Client();

        void toConnect(int port) override;
        int run() override;
    };

#endif