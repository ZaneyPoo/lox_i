#ifndef ZBASIC_UTILS_H_
#define ZBASIC_UTILS_H_

#define LOGGING_ENABLE

typedef enum LogLevel
{
    LOG_LVL_TRACE = 0,
    LOG_LVL_DEBUG,
    LOG_LVL_INFO,
    LOG_LVL_WARN,
    LOG_LVL_ERROR,
    LOG_LVL_FATAL,
} LogLevel;

void log_print(LogLevel lvl, const char* msg);

#ifdef LOGGING_ENABLE

#define LOG_TRACE(msg) log_print(LOG_LVL_TRACE, msg)
#define LOG_DEBUG(msg) log_print(LOG_LVL_DEBUG, msg)
#define LOG_INFO(msg) log_print(LOG_LVL_INFO, msg)
#define LOG_WARN(msg) log_print(LOG_LVL_WARN, msg)
#define LOG_ERR(msg) log_print(LOG_LVL_ERROR, msg)
#define LOG_FATAL(msg) log_print(LOG_LVL_FATAL, msg)

#else

#define LOG_TRACE(msg) 
#define LOG_DEBUG(msg) 
#define LOG_INFO(msg) 
#define LOG_WARN(msg) 
#define LOG_ERR(msg) 
#define LOG_FATAL(msg) 

#endif // LOGGING_ENABLE

#endif // ZBASIC_UTILS_H_
