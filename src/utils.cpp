#include "headers/utils.hpp"

Logger::Logger(const char* fileName){
    char *tail = "_logger.log";
    char newStr[strlen(fileName) + strlen(tail)];
    strcpy(newStr, fileName);
    strcat(newStr, tail);

    logFile.open(newStr);
    if (!logFile.is_open()){
        output("Error creating logger");
        exit(EXIT_FAILURE);
    }

    log("Description \t| Size   \t| Status");
    log("------------------------------------");
}

Logger::~Logger(){
    if (logFile.is_open()){
        logFile.close();
    }
}

void Logger::log(const char* message, int len){
    logFile << message << "\t| " << len << "bytes\t| " << strerror(errno) << endl;
}

void Logger::log(const char* message){
    log(message, 0);
}

void Logger::output(const char* message){
    cout << message << endl;
}
