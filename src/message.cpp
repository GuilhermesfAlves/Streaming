#include "include/message.hpp"

#define SIZE_OF_BYTE_IN_BITS 8
#define MOST_SIGNIFICANT_BIT 0b10000000 //128
#define CRC_POLYNOMIAL_SERVER 0x07 // 7
#define CRC_POLYNOMIAL_CLIENT 0x31 // 49

unsigned int Message::frameCounter;
unsigned char Message::crc_table_sender[POSSIBLE_VALUES_OF_A_BYTE];
unsigned char Message::crc_table_receiver[POSSIBLE_VALUES_OF_A_BYTE];
Message* Message::instance = NULL;

Message::Message(char operationMode){
    switch (operationMode){
    case CLIENT_MODE:
        crc_sender = CRC_POLYNOMIAL_CLIENT;
        crc_receiver = CRC_POLYNOMIAL_SERVER;
        break;
    case SERVER_MODE:
        crc_sender = CRC_POLYNOMIAL_SERVER;
        crc_receiver = CRC_POLYNOMIAL_CLIENT;
        break;
    }
    message = NULL;
    makeCrcTables();
    frameCounter = 0;
}

Message* Message::instanceOf(char operationMode){
    return (instance)? instance : (instance = new Message(operationMode));
}

msg_t* Message::deserializeMessage(const char type, const char* data){
    message = static_cast<msg_t*>(calloc(MAX_MESSAGE_SIZE, 1));

    if (!isValidType(type))
        return NULL;
    message -> head = HEAD_MARK;
    message -> frame = frameCounter++;
    message -> type = type;
    if (data == NULL){
        message -> size = 0;
        message -> data[0] = buildCrcSender(message -> size + 3);
        return message;
    }
    message -> size = strlen(data);
    strncpy(message -> data, data, message -> size);
    message -> data[message -> size] = buildCrcSender(msglen(message) - 1);
    
    return message;
}

int Message::serializeMessage(char* frame, char* type, char* data){
    *frame = message -> frame;
    *type = message -> type;
    strcpy(data, message -> data);

    return 1;
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

bool Message::isOppositeCrc(msg_t* msg){
    char expectedCrc = 0x00;
    unsigned char currentCrc = buildCrcSender(msglen(msg), msg);
    return (expectedCrc == currentCrc);
}


bool Message::isValidCrc(){
    char expectedCrc = 0x00;
    unsigned char currentCrc = buildCrcReceiver(msglen(message));
    return (expectedCrc == currentCrc);
}

char Message::buildCrcSender(int size, msg_t* msg){
    unsigned char crc = 0;

    // i = 1, pois o crc não deve validar o HEAD
    for (int i = 1; i < size; i++){
        crc = crc_table_sender[crc ^ (unsigned char)msg -> body[i]];
    }
    return crc;
}

char Message::buildCrcSender(int size){
    return buildCrcSender(size, message);
}

char Message::buildCrcReceiver(int size){
    unsigned char crc = 0;

    // i = 1, pois o crc não deve validar o HEAD
    for (int i = 1; i < size; i++){
        crc = crc_table_receiver[crc ^ (unsigned char)message -> body[i]];
    }
    return crc;
}

void Message::makeCrcTables(){

    for (int i = 0; i < POSSIBLE_VALUES_OF_A_BYTE; ++i) {
        unsigned char crc_s = i;
        unsigned char crc_r = i;
        for (char j = 0; j < SIZE_OF_BYTE_IN_BITS; ++j) {
            if (crc_s & MOST_SIGNIFICANT_BIT) 
                crc_s = (crc_s << 1) ^ crc_sender;
            else 
                crc_s <<= 1;

            if (crc_r & MOST_SIGNIFICANT_BIT) 
                crc_r = (crc_r << 1) ^ crc_receiver;
            else 
                crc_r <<= 1;
        }
        crc_table_sender[i] = crc_s;
        crc_table_receiver[i] = crc_r;
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
    if ((!strlen(msg)) || (msg[0] != HEAD_MARK) || (isOppositeCrc((msg_t*) msg))){
        message = NULL;
        return NOT_A_MESSAGE;
    }
    message = msgdup((msg_t*)msg);
    if (isValidCrc() && isValidType()){
        //remove o crc da mensagem
        message -> body[msglen((msg_t*)msg) - 1] = '\0';
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
        if ((message -> data[i] < '0') || (message -> data[i] > '9'))
            return INEXISTENT_FRAME;
    
    return atoi(message -> data);
}

int msglen(msg_t* msg){
    if (!msg)
        return 0;
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

    for (int i = 0; i < n; i++){
        if (m1 -> body[i] != m2 -> body[i]){
            return 1;
        }
    }
    return 0;
}