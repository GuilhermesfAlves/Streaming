#ifndef __LOGGER__
#define __LOGGER__

#include "utils.hpp"

class Logger{
private:
    ofstream logFile;
public:
    Logger(string fileName);
    ~Logger();
    void log(const char* message, const char* from);
    void output(const char* message);
};

#endif