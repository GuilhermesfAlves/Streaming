#ifndef __FLUX_CONTROL__
#define __FLUX_CONTROL__

#include <exception>

#include "socket.hpp"
#include "message.hpp"

#define OPTIONAL_TIMEOUT -2 // Don't throw error if not listen
#define INFINIT_TIMEOUT -1 // Wait till receive something
#define SHORTEST_TIMEOUT 0 
#define SHORT_TIMEOUT 1 // TIMEOUT MILLIS * 2
#define DEFAULT_TIMEOUT 2 // TIMEOUT MILLIS * 4
#define LONG_TIMEOUT 4 // TIMEOUT MILLIS * 16

#define TIMEOUT_MILLIS 5 

#define TIMEOUT_TOLERATION 3 // Duplicate timeout 3 times, duplicate per iteration

class FluxControl{
protected:
    static Message* message;
    static MySocket* socket;
    static char lastFrame;
    long long timestamp();
public: 
    FluxControl(string socketType);
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