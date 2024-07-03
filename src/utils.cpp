#include "headers/utils.hpp"

Logger::Logger(string fileName){
    logFile.open(fileName.append("_logger.log"));
    if (!logFile.is_open()){
        output("Error creating logger");
        exit(EXIT_FAILURE);
    }

    logFile << "Description \t| Size   \t| Status" << endl;
    logFile << "------------------------------------" << endl;
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
