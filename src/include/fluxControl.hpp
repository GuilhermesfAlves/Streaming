#ifndef __FLUX_CONTROL__
#define __FLUX_CONTROL__

#include <exception>
#include <list>

#include "socket.hpp"
#include "message.hpp"

#define OPTIONAL_TIMEOUT -2 // Don't throw error if not listen
#define INFINIT_TIMEOUT -1 // Wait till receive something
#define SHORTEST_TIMEOUT 0 // TIMEOUT MILLIS * 1
#define SHORT_TIMEOUT 1 // TIMEOUT MILLIS * 2
#define DEFAULT_TIMEOUT 2 // TIMEOUT MILLIS * 4
#define LONG_TIMEOUT 4 // TIMEOUT MILLIS * 16

#define TIMEOUT_MILLIS 50

#define TIMEOUT_TOLERATION 3 // Duplicate timeout 3 times, duplicate per iteration

#define FILE_OPEN_SUCCESS 0
#define FILE_FULL_DISK 1
#define FILE_OPEN_FAIL 2

#define COLLECTED_HISTORIC_SIZE 16

class FluxControl{
protected:
    static Message* message;
    static MySocket* socket;
    static list<msg_t*> collected;
    static char lastFrame;
    long long timestamp();
    void addCollectHistoric(msg_t* msg);
    char* alreadyCollected(char* buffer);
public: 
    FluxControl(string socketType, char operationMode);
    static void connect(int ifindex);
    int confirmAck(unsigned char frameToConfirm);
    int respond(unsigned char frameToConfirm, char type);
};

class TimeoutException: public exception{
private:
    string messageException = "Error Timeout Exception: ";
    int timeout;
public:
    explicit TimeoutException(int timeout);
    virtual const char* what() const noexcept override;
};

#endif
