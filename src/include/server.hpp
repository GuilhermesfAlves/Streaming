#ifndef SERVER
#define SERVER

    #include "streaming.hpp"

    class Server: public Streaming{
    private:
        const string path = "/cathalogS";
        ifstream* file;
        void getFilesInCathalog();
        void getFilesInCathalog(string path);
    public:
        Server();

        int run() override;
    };

#endif