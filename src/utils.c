#include "../incl/utils.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOCAL_BUF_SIZE 1024
#define MAX_STR_SIZE 101

#define STR_(x) #x
#define STR(x) STR_(x)

#define ESC \x1b
#define ANSI_RESET ESC[0m
#define ANSI_BOLD ESC[1m
#define ANSI_RED ESC[31m
// Oh god what have I done
// Macros are the Opium of the C masses

#define WRAP(str, seq...) STR(ANSI_##seq) str STR(ANSI_RESET)

static const char LOG_FORMAT_SPEC[] = "%s:%d: %s: \"%s\": %s\n"; 
static LogLevel LOG_OUTPUT_LVL = LOG_LVL_INFO;

#define LIT_DEF(lvl, display) {LOG_LVL_##lvl, #lvl, display}
static struct 
{
    LogLevel lvl;
    const char* compare;
    const char* display;
} LIT_LOG_LVLS[] = {
    LIT_DEF(TRACE, WRAP("[TRACE]", BOLD)),
    LIT_DEF(DEBUG, WRAP("[DEBUG]", BOLD)),
    LIT_DEF(INFO, WRAP("[INFO]", BOLD)),
    LIT_DEF(WARN, WRAP(WRAP("[WARN]", BOLD), RED)),
    LIT_DEF(ERROR, WRAP(WRAP("[ERROR]", BOLD), RED)),
    LIT_DEF(FATAL, WRAP(WRAP("[FATAL]", BOLD), RED)),
    {NUM_LOG_LVLS, "You shouldn't be here!", "SOMETHING'S WRONG"},
};

#undef LIT_DEF
#undef WRAP
#undef STR_
#undef STR


void log_print(LogLevel lvl, const char* msg)
{
    if (lvl >= LOG_OUTPUT_LVL)
        fprintf(stderr,
                LOG_FORMAT_SPEC,
                __FILE__, __LINE__, LIT_LOG_LVLS[lvl].display, __PRETTY_FUNCTION__, msg);
}



void log_set_output_lvl(LogLevel lvl)
{
    LOG_OUTPUT_LVL = lvl;
}



LogLevel log_get_output_lvl(void)
{
    return LOG_OUTPUT_LVL;
}



LogLevel log_name_to_lvl(const char* name) 
{
    for (LogLevel i = 0; i < NUM_LOG_LVLS; ++i)
    {
        if (strcmp(name, LIT_LOG_LVLS[i].compare) == 0)
            return i;
    }

    return LOG_LVL_INVALID;
}



static void prompt_keypress(void)
{
    printf("Press enter to continue...\n");
    getchar();
}



// len is assumed to include space for the null terminator
InputStr InputStr_new(const char* src, size_t len)
{
    InputStr input_str = {
        .text = malloc(sizeof(char) * len),
        .len = len
    };

    strncpy(input_str.text, src, len);

    return input_str;
}



InputStr InputStr_from_file(const char* filename)
{
    FILE* file_connection = fopen(filename, "r");
    InputStr input_str = {0};

    if (!file_connection)
    {
        LOG_ERR("Cannot open file");
        perror(filename);
        return (InputStr){NULL, 0};
    }

    fseek(file_connection, 0L, SEEK_END);
    input_str.len= ftell(file_connection);
    rewind(file_connection);

    input_str.text = malloc(sizeof(char) * input_str.len);

    if (!input_str.text)
    {
        LOG_ERR("Allocation for input file failed");
        fclose(file_connection);

        return (InputStr){NULL, 0};
    }

    fread(input_str.text, sizeof(char), input_str.len, file_connection);

    fclose(file_connection);
    return input_str;
}



InputStr InputStr_from_stdin(const char prompt_msg[], size_t max)
{
    char buf[LOCAL_BUF_SIZE] = {0};
    InputStr input_str = {0};
    size_t len = 0; 

    if (max <= 0)
        max = MAX_STR_SIZE;

    do 
    {
        printf("%s", prompt_msg);
        if (!fgets(buf, LOCAL_BUF_SIZE, stdin) || feof(stdin))
            return input_str;

        buf[strcspn(buf, "\n")] = 0; // remove newline if present
        len = strlen(buf) + 1;

        if (len <= 1) 
        {

            fprintf(stderr, 
                    "\nNo characters entered. Please enter a command."
                    "\nEnter \"help\" for a list of commands.\n");
            prompt_keypress();

        }
        else if (len > max)
        {
            fprintf(stderr, 
                    "\nToo many characters!"
                    "\n%d characters max. You entered %d characters.\n",
                    (int)max, (int)len);
            prompt_keypress();

        }
    } while (len <= 1 || len > max);

    input_str.text = malloc(sizeof(char) * len);
    strncpy(input_str.text, buf, len);

    return input_str;
}



void InputStr_delete(InputStr* input_str)
{
    if (input_str) free(input_str->text);
    input_str->len = 0;
}
