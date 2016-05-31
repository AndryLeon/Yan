
#ifndef YAN_LOG_H
#define YAN_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Yan{

    enum LogLevel{
        NOTICE, TRACE, DEBUG, WARN, FATAL
    };

    extern LogLevel logLevel;

    inline LogLevel GetLogLevel(){
        return logLevel;
    }

    inline void SetLogLevel(LogLevel l){
        logLevel = l;
    }

#define LOG_NOTICE(fmt, ...) if(Yan::GetLogLevel() <= Yan::NOTICE) \
        fprintf(stderr, "[NOTICE][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_TRACE(fmt, ...) if(Yan::GetLogLevel() <= Yan::TRACE) \
        fprintf(stderr, "[TRACE][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) if(Yan::GetLogLevel() <= Yan::DEBUG) \
        fprintf(stderr, "[DEBUG][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) if(Yan::GetLogLevel() <= Yan::WARN) \
        fprintf(stderr, "[WARN][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_FATAL(fmt, ...) if(Yan::GetLogLevel() <= Yan::FATAL) \
        fprintf(stderr, "[FATAL][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
}

#endif //YAN_LOG_H

