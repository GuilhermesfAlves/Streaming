#include "headers/message.hpp"

#define SIZE_OF_BYTE_IN_BITS 8
#define MOST_SIGNIFICANT_BIT 0b10000000 //128
#define CRC_POLYNOMIAL 0b00000111 //7


unsigned int Message::sequence;
char Message::crc_table[POSSIBLE_VALUES_OF_A_BYTE];

msg_t* Message::deserializeMessage(const char type, const char* data){
    message = static_cast<msg_t*>(calloc(MAX_MESSAGE_SIZE, 1));

    if (!isValidType(type))
        return NULL;
    message -> head = HEAD_MARK;
    message -> seq = sequence++;
    message -> type = type;
    if (data == NULL){
        message -> size = 0;
        message -> data[0] = buildCrc();
        return message;
    }
    message -> size = strlen(data);
    strncpy(message -> data, data, message -> size);
    
    return message;
}

int Message::serializeMessage(char* seq, char* type, char* data){
    *seq = message -> seq;
    *type = message -> type;
    strcpy(data, message -> data);

    return 1;
}

char* Message::getMessage(){
    return message -> m;
}

bool Message::isValidType(){
    return isValidType(message -> type);
}

bool Message::isValidType(const char type){
    
    switch (type){
    case T_ACK:
    case T_NACK:
    case T_LIST:
    case T_DOWNLOAD:
    case T_PRINT:
    case T_FILE_DESCRIPTOR:
    case T_DATA: 
    case T_END_TX:
    case T_ERROR:
        return true;
    default:
        return false;
    }
}

bool Message::isValidCrc(){
    char expectedCrc = 0x00;
    char currentCrc = buildCrc();
    
    return (expectedCrc == currentCrc);
}

char Message::buildCrc(){
    char crc = 0;

    // i = 1, pois o crc não deve validar o HEAD
    for (int i = 1; i < static_cast<int>(strlen(message -> m)); i++)
        crc = crc_table[crc ^ message -> m[i]];

    return crc;
}

Message::Message(){
    message = NULL;
    makeCrcTable();
    Message::sequence = 0;
}

void Message::makeCrcTable(){

    for (int i = 0; i < POSSIBLE_VALUES_OF_A_BYTE; ++i) {
        char crc = i;
        for (char j = 0; j < SIZE_OF_BYTE_IN_BITS; ++j) {
            if (crc & MOST_SIGNIFICANT_BIT) {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
        Message::crc_table[i] = crc;
    }
}

char* Message::getData(){
    return message -> data;
}

char Message::getType(){
    return message -> type;
}

char Message::getSeq(){
    return message -> seq;
}

void Message::setMessage(char* msg){
    if (message){
        free(message);
    }
    message = (msg_t*)strdup(msg);
    if (isValidCrc()){
        message -> m[message -> size + 3] = '\0';
    }
}