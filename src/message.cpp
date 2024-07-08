#include "headers/message.hpp"

#define SIZE_OF_BYTE_IN_BITS 8
#define MOST_SIGNIFICANT_BIT 0b10000000 //128
#define CRC_POLYNOMIAL 0b00000111 //7


unsigned int Message::frameCounter;
unsigned char Message::crc_table[POSSIBLE_VALUES_OF_A_BYTE];

msg_t* Message::deserializeMessage(const char type, const char* data){
    message = static_cast<msg_t*>(calloc(MAX_MESSAGE_SIZE, 1));

    if (!isValidType(type))
        return NULL;
    message -> head = HEAD_MARK;
    message -> frame = frameCounter++;
    message -> type = type;
    if (data == NULL){
        message -> size = 0;
        message -> data[0] = buildCrc(message -> size + 3);
        return message;
    }
    message -> size = strlen(data);
    strncpy(message -> data, data, message -> size);
    message -> data[message -> size] = buildCrc(message -> size + 3);
    cout << (int) (unsigned char)message -> data[message -> size + 1] << endl;
    
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
    unsigned char currentCrc = buildCrc(message -> size + 4);
    cout << "current crc: " << (int) currentCrc << endl;
    cout << "is valid crc: " << (expectedCrc == currentCrc) << endl;
    return (expectedCrc == currentCrc);
}

char Message::buildCrc(int size){
    unsigned char crc = 0;

    // i = 1, pois o crc não deve validar o HEAD
    cout << "tam:" << size << endl;
    for (int i = 1; i < size; i++){
        crc = crc_table[(unsigned char)crc ^ (unsigned char)message -> m[i]];
    }

    return crc;
}

Message::Message(){
    message = NULL;
    makeCrcTable();
    frameCounter = 0;
}

void Message::makeCrcTable(){

    for (int i = 0; i < POSSIBLE_VALUES_OF_A_BYTE; ++i) {
        unsigned char crc = i;
        for (char j = 0; j < SIZE_OF_BYTE_IN_BITS; ++j) {
            if (crc & MOST_SIGNIFICANT_BIT) {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
        crc_table[i] = crc;
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
    message = (msg_t*)strndup(msg, MAX_MESSAGE_SIZE);
    if (isValidCrc() && isValidType()){
        cout << "is valid message" << endl; 
        // message -> m[message -> size + 3] = '\0';
    } else {
        message = NULL;
    }
}