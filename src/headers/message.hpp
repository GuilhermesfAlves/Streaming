#ifndef __MESSAGE__
#define __MESSAGE__

#include "utils.hpp"

#define POSSIBLE_VALUES_OF_A_BYTE 256

#define MAX_MESSAGE_SIZE 67
#define MAX_DATA_SIZE 63

#define INEXISTENT_FRAME 32
#define MAX_FRAME 31
#define MIN_FRAME 0

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
        char head;
        unsigned char size : 6;
        unsigned char frame : 5;
        unsigned char type : 5;
        char data[MAX_DATA_SIZE];
        char crc; 
    };
    char m[MAX_MESSAGE_SIZE];
} msg_t;
#pragma pack(pop)

class Message{
private:
    msg_t* message;
    unsigned int static frameCounter;
    static unsigned char crc_table[POSSIBLE_VALUES_OF_A_BYTE];
public:
    Message();
    //construct message
    msg_t* deserializeMessage(const char type, const char* data);
    //desconstruct message
    int serializeMessage(char* frame, char* type, char* data);
    char* getMessage();
    void setMessage(char* msg);
    char* getData();
    char getType();
    char getFrame();
    char getMessageSize();
private:
    bool isValidType();
    bool isValidType(const char type);
    bool isValidCrc();
    char buildCrc(int size);
    void makeCrcTable();
};

#endif 