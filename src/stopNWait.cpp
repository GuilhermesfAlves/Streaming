#include "include/stopNWait.hpp"

StopNWait::StopNWait(string socketType) : FluxControl(socketType){}

int StopNWait::receive(int timeout){
    int status;

    int i = 0;
    while(((status = listen(timeout << i)) == NOT_A_MESSAGE) && (++i < TIMEOUT_TOLERATION));

    if ((status == NOT_A_MESSAGE) && (timeout < OPTIONAL_TIMEOUT))
        throw TimeoutException(timeout);

    if ((status == NOT_A_MESSAGE) && (timeout >= OPTIONAL_TIMEOUT))
        return 0;

    if (!isExpectedFrame()){
        status = INVALID_MESSAGE;
    }
    
    if (status == VALID_MESSAGE){
        updateLastReceived(message -> getMessage());
        if (message -> getType() == T_PRINT)
            cout << "\t" << message -> getData() << endl;
    }

    return marshallACK(status);
}

void StopNWait::send(unsigned char type, const char* msg){
    msg_t* toSend = message -> buildMessage(type, msg);
    int status;
    int i = 0;
    addSentHistoric(toSend);
    do {
        socket -> post(toSend, msglen(toSend));
    } while((((status = listen(SHORT_TIMEOUT << i)) == NOT_A_MESSAGE) || !confirmAck(toSend -> frame)) && (++i < TIMEOUT_TOLERATION));

    if (i >= TIMEOUT_TOLERATION)
        throw TimeoutException(SHORT_TIMEOUT);

    updateLastReceived(message -> getMessage());
}

void StopNWait::send(unsigned char type, int msg){
    char buffer[BUFFER_SIZE] = {0};
    sprintf(buffer, "%d", msg);
    send(type, buffer);
}

void StopNWait::send(unsigned char type){
    send(type, (char*)NULL);
}

void StopNWait::send(ifstream* file, string path){
    char buffer[BUFFER_SIZE] = {0}; 
    auto ftime = filesystem::last_write_time(path);
    
    // Converter para time_t para fácil manipulação
    auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
        ftime - filesystem::file_time_type::clock::now() + chrono::system_clock::now()
    );
    time_t cftime = chrono::system_clock::to_time_t(sctp);

    // Converter para struct tm para quebrar a data e hora
    tm* local_time = localtime(&cftime);
    
        // Imprimir no formato AAAAMMDDHHMM
    ostringstream oss;
    oss << put_time(local_time, "%Y%m%d%H%M");
    sprintf(buffer, "%ld,%s", static_cast<long>(file -> tellg()), oss.str().c_str());
    send(T_FILE_DESCRIPTOR, buffer);
}

int StopNWait::getFileSize(){
    char* msg = message -> getData();

    return atoi(strtok(msg, ","));
}

tm StopNWait::getFileData(){
    char* msg = message -> getData();
    strtok(msg, ",");
    string data = strtok(msg, ",");
    
    tm tm = {};
    istringstream ss(data);
    ss >> get_time(&tm, "%Y%m%d%H%M");

    return tm;
}

char* StopNWait::getDataStr(){
    return message -> getData();
}
