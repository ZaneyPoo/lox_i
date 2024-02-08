#ifndef ZLOX_UTILS_H_
#define ZLOX_UTILS_H_
#include <stddef.h>


typedef enum LogLevel
{
    LOG_LVL_TRACE = 0,
    LOG_LVL_DEBUG,
    LOG_LVL_INFO,
    LOG_LVL_WARN,
    LOG_LVL_ERROR,
    LOG_LVL_FATAL,
    NUM_LOG_LVLS,
    LOG_LVL_INVALID,
} LogLevel;

#define LOGGING_ENABLE

// TODO: flesh this out and actually use it

void log_print(LogLevel lvl, const char* msg);

void log_set_output_lvl(LogLevel lvl);

LogLevel log_get_output_lvl(void);

LogLevel log_name_to_lvl(const char* name);

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


typedef struct InputStr 
{
    char* text;
    size_t len;
} InputStr;


InputStr InputStr_new(const char* src, size_t len);

InputStr InputStr_from_file(const char* filename);

InputStr InputStr_from_stdin(const char prompt_msg[], size_t max);

void InputStr_delete(InputStr* input_string);

#endif // ZLOX_UTILS_H_
