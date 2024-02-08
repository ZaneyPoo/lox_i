#include "../incl/utils.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOCAL_BUF_SIZE 1024
#define MAX_STR_SIZE 101

#define ESC 0x1b
#define ANSI_RESET "ESC[0m"
#define ANSI_BOLD "ESC[1m"
#define ANSI_RED "ESC[31"

#define WRAP(str, seq...) ANSI_##seq str ANSI_RESET

// Oh god what have I done
// Macros are the Opium of the C masses
static const char* LOG_LVL_NAMES[] = {
    WRAP("[TRACE]", BOLD),
    WRAP("[DEBUG]", BOLD),
    WRAP("[INFO]", BOLD),
    WRAP(WRAP("[WARN]", BOLD), RED),
    WRAP(WRAP("[ERROR]", BOLD), RED),
    WRAP(WRAP("[FATAL]", BOLD), RED),
};
static const char LOG_FORMAT_SPEC[] = "%s:%d: %s: \"%s\": %s\n"; 



void log_print(LogLevel lvl, const char* msg)
{
    fprintf(stderr,
            LOG_FORMAT_SPEC,
            __FILE__, __LINE__, LOG_LVL_NAMES[lvl], __PRETTY_FUNCTION__, msg);
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
