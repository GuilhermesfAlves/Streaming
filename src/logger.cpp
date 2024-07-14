#include "include/logger.hpp"
#include <iomanip>

Logger::Logger(string fileName){
    logFile.open(fileName.append("_logger.log"));
    if (!logFile.is_open()){
        cout << "Error creating logger" << endl;
        exit(EXIT_FAILURE);
    }

    logFile << "From \t| Size \t| Frm \t| Type \t|" <<\
        " Data" << setw(63) << setfill(' ') << " \t| Status" << endl;
    logFile << "------------------------------------------------------------------"
     "-----------------------------------------------------------" << endl;
}

Logger::~Logger(){
    if (logFile.is_open())
        logFile.close();
}

void Logger::log(const char* message, const char* from){
    msg_t* m = (msg_t*)(message);
    char data[MAX_DATA_SIZE] = {0};
    strncpy(data, m -> data, m -> size);

    logFile << "\t" << from \
     << "\t| " << setw(2) << setfill('0') << (int)(m -> size)\
      << "\t| " << setw(2) << setfill('0') << (int)(m -> frame)\
       << "\t| " << setw(2) << setfill('0') << (int)(m -> type)\
        << "\t| " << setw(63) << setfill(' ') << data \
         << "\t| " << strerror(errno) << endl;
}