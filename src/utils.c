#include "../incl/utils.h"
#include <stdio.h>

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


