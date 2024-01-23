#include "../incl/prompt.h"
#include "../incl/utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOCAL_BUF_SIZE 1024
#define MAX_STR_SIZE 101




static void prompt_keypress(void)
{
    printf("Press enter to continue...\n");
    getchar();
}



char* prompt_cstr(const char prompt_msg[], size_t max)
{
    char buf[LOCAL_BUF_SIZE] = {};
    char* cstr = NULL;
    size_t len = 0; 

    if (max <= 0)
        max = MAX_STR_SIZE;

    do 
    {
        printf("%s", prompt_msg);
        fgets(buf, LOCAL_BUF_SIZE, stdin);
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

    cstr = malloc(sizeof(char) * len);
    strncpy(cstr, buf, len);

    return cstr;
}



int start_interative_mode(void)
{
    // TODO: for the command line prompt mode
    LOG_ERR("Unimplemented function");
    return 0;
}
