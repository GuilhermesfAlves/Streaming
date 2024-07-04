#ifndef __MESSAGE__
#define __MESSAGE__

#include "utils.hpp"

#define POSSIBLE_VALUES_OF_A_BYTE 256
#define MAX_MESSAGE_SIZE 68
#define MAX_DATA_SIZE 64

#define T_ACK 0b00000
#define T_NACK 0b00001
#define T_LIST 0b01010
#define T_DOWNLOAD 0b01011
#define T_PRINT 0b10000
#define T_FILE_DESCRIPTOR 0b10001
#define T_DATA 0b10010
#define T_END_TX 0b11110
#define T_ERROR 0b11111

typedef union{
    struct {
        char head;
#pragma pack(push, 1)
        union{
            struct{
                unsigned char size : 6;
                unsigned char seq : 5;
                unsigned char type : 5;
                };
            short sst;
        };
#pragma pack(pop)
        char data[MAX_DATA_SIZE];
        char crc; 
    };
    char m[MAX_MESSAGE_SIZE];
} msg_t;

class Message{
private:
    msg_t* message;
    unsigned int static sequence;
    static char crc_table[POSSIBLE_VALUES_OF_A_BYTE];
public:
    Message();
    //construct message
    msg_t* deserializeMessage(const char type, const char* data);
    //desconstruct message
    int serializeMessage(char* seq, char* type, char* data);
    char* getMessage();
    void setMessage(char* msg);
    char* getData();
    char getType();
    char getSeq();
private:
    bool isValidType();
    bool isValidType(const char type);
    bool isValidCrc();
    char buildCrc();
    void makeCrcTable();
};

#endif 