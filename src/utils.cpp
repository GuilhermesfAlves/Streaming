#include "headers/utils.hpp"

Logger::Logger(string fileName){
    logFile.open(fileName.append("_logger.log"));
    if (!logFile.is_open()){
        output("Error creating logger");
        exit(EXIT_FAILURE);
    }

    logFile << "Size \t| Sequence \t| Type \t| Data \t| Status" << endl;
    logFile << "------------------------------------" << endl;
}

Logger::~Logger(){
    if (logFile.is_open()){
        logFile.close();
    }
}

void Logger::log(char* message){
    msg_t* m = reinterpret_cast<msg_t*>(message);
    logFile << m -> size << "\t| " << m -> seq << "\t| " << m -> type << "\t| " << m -> data << "\t| " << strerror(errno) << endl;
}

void Logger::output(const char* message){
    cout << message << endl;
}
