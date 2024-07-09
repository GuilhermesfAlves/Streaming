#include "headers/utils.hpp"

int myAtoi(char* str){

    for (int i = 0; str[i] != '\0'; i++){
        if ((str[i] < '0') || (str[i] > '9'))
            return INEXISTENT_FRAME;
    }
    return atoi(str);
}

int myStrlen(msg_t* msg){
    return msg -> size;
}

void myStrcpy(char* dest, char* src, int size){
    for (int i = 0; i < size; i++)
        new_msg[i] = msg[i];    
}

char* myStrdup(char* msg){
    int size = 0;

    //myStrlen
    for (int i = 0; i < MAX_MESSAGE_SIZE; i++)
        if (msg[i]!= '\0')
            size = i;
    
    cout << "My strdup" << endl;
    size++;
    char* new_msg = (char*) calloc(0, (size_t)size);
    //myStrcpy
    for (int i = 0; i < size; i++)
        new_msg[i] = msg[i];    
    
    
    return new_msg;
}


