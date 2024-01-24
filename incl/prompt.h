#ifndef ZLOX_PROMPT_H_
#define ZLOX_PROMPT_H_
#include <stddef.h>

char* prompt_cstr(const char prompt_msg[], size_t max);

void print_err(const char error_msg[]);

int start_interative_mode(void);

#endif // ZLOX_PROMPT_H_
