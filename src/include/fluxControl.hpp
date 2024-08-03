#ifndef __FLUX_CONTROL__
#define __FLUX_CONTROL__

#include <exception>
#include <list>

#include "socket.hpp"
#include "message.hpp"

#define OPTIONAL_TIMEOUT 513 // Don't throw error if not listen
#define INFINIT_TIMEOUT 0 // Wait till receive something
#define SHORTEST_TIMEOUT 1 // TIMEOUT MILLIS * 2
#define SHORT_TIMEOUT 2 // TIMEOUT MILLIS * 4
#define DEFAULT_TIMEOUT 3 // TIMEOUT MILLIS * 8
#define LONG_TIMEOUT 4 // TIMEOUT MILLIS * 16

#define TIMEOUT_MILLIS 1

#define TIMEOUT_TOLERATION 3 // Duplicate timeout 3 times, duplicate per iteration

#define NACK_TOLERATION 5

#define SENDING_NACK 0
#define RECEIVING_NACK 1

#define FILE_OPEN_SUCCESS 0
#define FILE_OPEN_FAIL 1
#define FILE_NOT_FOUND 2
#define FILE_FULL_DISK 3

#define COLLECTED_HISTORIC_SIZE 16

class FluxControl{
private: 
    int getTime(int timeout);
protected:
    static Message* message;
    static Socket* socket;
    static list<msg_t*> collected;
    static list<msg_t*> sent;
    static list<msg_t*> nackList;
    static unsigned char lastReceivedFrame;
    int marshallACK(int status);
    long long timestamp();
    void addCollectHistoric(msg_t* newMsg);
    void addSentHistoric(msg_t* newMsg);
    char* inContext(char* buffer);
    int listen(int timeout);
    void updateLastReceived(msg_t * msg);
    bool isExpectedFrame();
public: 
    FluxControl(string socketType);
    static void connect(int ifindex);
    int confirmAck(unsigned char frameToConfirm);
    int confirmAck();
    void flushHistoric();
    int respond(char type);
};

class TimeoutException: public exception{
private:
    string messageException = "Error Timeout Exception: ";
    int timeout;
public:
    explicit TimeoutException(int timeout);
    virtual const char* what() const noexcept override;
};

class BadConnectionException: public exception{
private:
    string messageException = "Bad Connection Exception, ";
    msg_t* badMessage = NULL;
    bool isSending;
public:
    explicit BadConnectionException(msg_t* badMessage, bool isSending);
    virtual const char* what() const noexcept override;
};

#endif
