#ifndef CLIENT
#define CLIENT

    #include "streaming.hpp"

    class Client: public Streaming{
    public:
        Client();

        int run() override;
    };

#endif