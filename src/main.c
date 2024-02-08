#include "../incl/utils.h"
#include "../incl/interpret.h"
#include <stdlib.h>
#include <getopt.h>

#define MAX_NUM_ARGS 10

const char USAGE[] = 
    "Usage: zli [OPTIONS] [<file>]\n"
    "       zli [OPTIONS] [-]\n"
    "Options:\n"
    "  -l LVL, --log-lvl=LVL  Minimum level of logging.\n"
    "                         [default: INFO]\n"
    "  -h, --help             Display this help message and exit.\n";

static const char SHORT_OPTIONS[] = "lh";
static struct option OPTIONS[MAX_NUM_ARGS] = {
    {
        .name = "log-lvl",
        .has_arg = required_argument,
    },
};




int main(int argc, char* argv[]) 
{
    InputStr input = {0};
    Lox lox = Lox_new();

    int opt_index = 0;
    char short_opt;
    struct option opts[MAX_NUM_ARGS] = {0};

    if (argc == 1)
    {
        return (Lox_repl(&lox) ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    // TODO: Implement option parsing
    // while (
    // (short_opt = getopt_long(argc, argv, SHORT_OPTIONS, OPTIONS, &opt_index)) != -1)
    // {
    // }

    const char* filename = argv[1];
    input = InputStr_from_file(filename);

    return (Lox_run(&lox, &input) ? EXIT_SUCCESS : EXIT_FAILURE);
}
