#include "headers/message.hpp"

#define SIZE_OF_BYTE_IN_BITS 8
#define MOST_SIGNIFICANT_BIT 0b10000000 //128
#define CRC_POLYNOMIAL 0b00000111 //7


unsigned int Message::frameCounter;
char Message::crc_table[POSSIBLE_VALUES_OF_A_BYTE];

msg_t* Message::deserializeMessage(const char type, const char* data){
    message = static_cast<msg_t*>(calloc(MAX_MESSAGE_SIZE, 1));

    if (!isValidType(type))
        return NULL;
    message -> head = HEAD_MARK;
    message -> frame = frameCounter++;
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

int Message::serializeMessage(char* frame, char* type, char* data){
    *frame = message -> frame;
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
    case T_INEXISTENT:
    default:
        return false;
    }
}

bool Message::isValidCrc(){
    char expectedCrc = 0x00;
    char currentCrc = buildCrc();
    cout << "is valid crc: " << (expectedCrc == currentCrc) << endl;
    return (expectedCrc == currentCrc);
}

char Message::buildCrc(){
    char crc = 0;

    // i = 1, pois o crc não deve validar o HEAD
    for (int i = 1; i < getMessageSize(); i++)
        crc = crc_table[crc ^ message -> m[i]];

    return crc;
}

Message::Message(){
    message = NULL;
    makeCrcTable();
    Message::frameCounter = 0;
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
    if (!message)
        return strdup("32");
    return message -> data;
}

char Message::getType(){
    if (!message)
        return T_INEXISTENT;
    return message -> type;
}

char Message::getFrame(){
    if (!message)
        return INEXISTENT_FRAME;
    return message -> frame;
}

char Message::getMessageSize(){
    if (!message)
        return 0;
    return message -> size + 4;
}

void Message::setMessage(char* msg){
    if (!strlen(msg)){
        cout << "mensagem vazia" << endl;
        if (message){
            message = NULL;
        }
        return;
    }

    message = (msg_t*)strdup(msg);
    if (isValidCrc() && isValidType()){
        cout << "is valid message" << endl; 
        message -> m[message -> size + 3] = '\0';
    } else {
        message = NULL;
    }
}