#ifndef SERVER
#define SERVER

    #include "streaming.hpp"

    class Server: public Streaming{
    public:
        Server();

        int run() override;
    };

#endif