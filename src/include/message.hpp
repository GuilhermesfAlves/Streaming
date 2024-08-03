#ifndef __MESSAGE__
#define __MESSAGE__

#include "utils.hpp"

#define POSSIBLE_VALUES_OF_A_BYTE 256

#define CLIENT_SOCKET_STR "Client"
#define SERVER_SOCKET_STR "Server"
#define CLIENT_MODE 0
#define SERVER_MODE 1

#define INVALID_MESSAGE -1
#define VALID_MESSAGE 1
#define NOT_A_MESSAGE 0

#define ERROR_ACCESS_DENIED 1
#define ERROR_FILE_NOT_FOUND 2
#define ERROR_FULL_DISK 3

#define MAX_MESSAGE_SIZE 67
#define MIN_MESSAGE_SIZE 4
#define MAX_DATA_SIZE 63
#define MIN_DATA_SIZE 0

#define OVERHEAD MIN_MESSAGE_SIZE

#define INEXISTENT_FRAME 32
#define MAX_FRAME 31
#define MIN_FRAME 0

#define DATA_INDEX 3

#define HEAD_MARK 0b01111110 //126

#define T_ACK 0b00000 //0
#define T_NACK 0b00001 //1
#define T_LIST 0b01010 //10
#define T_DOWNLOAD 0b01011 //11
#define T_PRINT 0b10000 //16
#define T_FILE_DESCRIPTOR 0b10001 //17
#define T_DATA 0b10010 //18
#define T_END_TX 0b11110 //30
#define T_ERROR 0b11111 //31
#define T_INEXISTENT 32

#pragma pack(push, 1)
typedef union{
    struct {
        unsigned char head;
        unsigned char size : 6;
        unsigned char frame : 5;
        unsigned char type : 5;
        char data[MAX_DATA_SIZE];
        unsigned char crc; 
    };
    char body[MAX_MESSAGE_SIZE];
} msg_t;
#pragma pack(pop)

class Message{
private:
    static Message* instance;
    msg_t* message;
    unsigned int static frameCounter;
    static unsigned char crc_table[POSSIBLE_VALUES_OF_A_BYTE];
    Message();
    void durty();
public:
    static Message* instanceOf();
    //construct message
    msg_t* buildMessage(const char type, const char* data);
    msg_t* buildMessage(const char type, const char* data, const int tam);
    msg_t* getMessage();
    char* getBody();
    int setMessage(char* msg);
    int setMessage(msg_t* msg);
    char* getData();
    char getType();
    char getFrame();
    int getSize();
    char getMessageSize();
    int dataAtoi();
private:
    bool isValidType();
    bool isValidType(const char type);
    bool isValidCrc();
    bool isValidCrc(msg_t* msg);
    char buildCrc(int size);
    char buildCrc(int size, msg_t* msg);
    void makeCrcTable();
};

int datalen(const char* data);
void datancpy(char* dest, const char* src, int n);
int msglen(msg_t* msg);
msg_t* msgdup(msg_t* msg);
msg_t* msgcpy(msg_t* dest, msg_t* src);
int msgncmp(msg_t* m1, msg_t* m2, int n);
string msgToString(msg_t* m);

#endif 
