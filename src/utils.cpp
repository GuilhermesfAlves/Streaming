#include "headers/utils.hpp"

Logger::Logger(string fileName){
    logFile.open(fileName.append("_logger.log"));
    if (!logFile.is_open()){
        output("Error creating logger");
        exit(EXIT_FAILURE);
    }

    logFile << "Size \t| Sequence \t| Type \t| Data \t| Status" << endl;
    logFile << "-----------------------------------------------" << endl;
}

Logger::~Logger(){
    if (logFile.is_open()){
        logFile.close();
    }
}

void Logger::log(char* message){
    msg_t* m = (msg_t*)(message);
    char data[MAX_DATA_SIZE] = {0};
    strncpy(data, m -> data, m -> size);

    logFile << (int)(m -> size)\
     << "\t| " << (int)(m -> seq)\
      << "\t| " << (int)(m -> type)\
       << "\t| " << data\
        << "\t| " << strerror(errno) << endl;
}

void Logger::output(const char* message){
    cout << message << endl;
}
