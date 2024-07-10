#include "include/logger.hpp"
#include <iomanip>

Logger::Logger(string fileName){
    logFile.open(fileName.append("_logger.log"));
    if (!logFile.is_open()){
        output("Error creating logger");
        exit(EXIT_FAILURE);
    }

    logFile << "From \t| Size \t| Frame \t| Type \t| Data \t| Status" << endl;
    logFile << "----------------------------------------------------" << endl;
}

Logger::~Logger(){
    if (logFile.is_open()){
        logFile.close();
    }
}

void Logger::log(const char* message, const char* from){
    // if (!strlen(message))
    //     return;

    msg_t* m = (msg_t*)(message);
    char data[MAX_DATA_SIZE] = {0};
    strncpy(data, m -> data, m -> size);

    logFile << "\t" << from \
     << "\t| " << setw(2) << setfill('0') << (int)(m -> size)\
      << "\t| " << setw(2) << setfill('0') << (int)(m -> frame)\
       << "\t| " << setw(2) << setfill('0') << (int)(m -> type)\
        << "\t| " << data\
         << "\t| " << strerror(errno) << endl;
}

void Logger::output(const char* message){
    cout << message << endl;
}
