#include "include/message.hpp"

#define SIZE_OF_BYTE_IN_BITS 8
#define MOST_SIGNIFICANT_BIT 0b10000000 //128
#define CRC_POLYNOMIAL 0x07 // 7

unsigned int Message::frameCounter;
unsigned char Message::crc_table[POSSIBLE_VALUES_OF_A_BYTE];
Message* Message::instance = NULL;

Message::Message(){
    message = NULL;
    makeCrcTable();
    frameCounter = 0;
    srand(time(NULL));
}

Message* Message::instanceOf(){
    return (instance)? instance : (instance = new Message());
}

msg_t* Message::buildMessage(const char type, const char* data){
    return buildMessage(type, data, datalen(data));
}

void Message::durty(){
    if (message -> size >= 14)
        return;
    for (int i = message -> size + 4; i < 14; i++)
        message -> body[i] = (rand() % 26) + 97;
}

msg_t* Message::buildMessage(const char type, const char* data, const int tam){
    message = static_cast<msg_t*>(calloc(MAX_MESSAGE_SIZE, 1));

    if (!isValidType(type))
        return NULL;

    message -> head = HEAD_MARK;
    message -> frame = frameCounter++;
    message -> type = type;
    if (data == NULL){
        message -> size = 0;
    } else {
        message -> size = tam;
        datancpy(message -> data, data, message -> size);
    }
    message -> data[message -> size] = buildCrc(message -> size + 3);
    durty();    
    return message;
}

int Message::getSize(){
    return message -> size;
}

msg_t* Message::getMessage(){
    return message;
}

char* Message::getBody(){
    return message -> body;
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
    return isValidCrc(message);
}

bool Message::isValidCrc(msg_t* msg){
    unsigned char expectedCrc = 0x00;
    unsigned char currentCrc = buildCrc(msg -> size + 4, msg);
    return (expectedCrc == currentCrc);
}

char Message::buildCrc(int size, msg_t* msg){
    unsigned char crc = 0;

    // i = 1, pois o crc não deve validar o HEAD
    for (int i = 1; i < size; i++){
        crc = crc_table[crc ^ (unsigned char)msg -> body[i]];
    }
    return crc;
}

char Message::buildCrc(int size){
    return buildCrc(size, message);
}

void Message::makeCrcTable(){

    for (int i = 0; i < POSSIBLE_VALUES_OF_A_BYTE; ++i) {
        unsigned char crc = i;
        for (char j = 0; j < SIZE_OF_BYTE_IN_BITS; ++j) {
            if (crc & MOST_SIGNIFICANT_BIT) 
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            else 
                crc <<= 1;
        }
        crc_table[i] = crc;
    }
}

char* Message::getData(){
    if (!message)
        return const_cast<char*>("32");
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
    return msglen(message);
}

int Message::setMessage(char* msg){
    if ((!msg) || (!strlen(msg)) || (msg[0] != HEAD_MARK)){
        return NOT_A_MESSAGE;
    }
    message = msgdup((msg_t*)msg);
    if (isValidCrc() && isValidType()){
        //remove o crc da mensagem
        message -> body[message -> size + 3] = '\0';
        return VALID_MESSAGE;
    }
    return INVALID_MESSAGE;
}

int Message::setMessage(msg_t* msg){
    if (msg){
        message = msg;
        return VALID_MESSAGE;
    }
    return INVALID_MESSAGE;
}

int Message::dataAtoi(){

    if (!message)
        return INEXISTENT_FRAME;

    for (int i = 0; message -> data[i] != '\0'; i++)
        if ((message -> data[i] < '0') || (message -> data[i] > '9')){
            return INEXISTENT_FRAME;
        }
    
    return atoi(message -> data);
}

int datalen(const char* data){
    int i = 0;
    for (;data && data[i] != '\0'; i++){}
    return i;
}

int msglen(msg_t* msg){
    if (!msg)
        return 0;

    if (msg -> size + OVERHEAD < 14)
        return 14;

    return msg -> size + OVERHEAD;
}

msg_t* msgdup(msg_t* msg){
    int size = msglen(msg);

    if (!size)
        return NULL;

    msg_t* new_msg = (msg_t*) calloc((size_t)size + 1, 1);
    
    return msgcpy(new_msg, msg);
}

msg_t* msgcpy(msg_t* dest, msg_t* src){
    int size = msglen(src);
    
    for (int i = 0; i < size; i++)
        dest -> body[i] = src -> body[i];

    return dest;
}

int msgncmp(msg_t* m1, msg_t* m2, int n){
    if ((msglen(m1) < n) || (msglen(m2) < n)){
        return 1;
    }

    for (int i = 0; i < n; i++)
        if ((m1 -> size + 3 != i) && (m1 -> body[i] != m2 -> body[i])){
            return 1;
        }
    return 0;
}

void datancpy(char* dest, const char* src, int n){
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}