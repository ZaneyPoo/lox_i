#include "../incl/utils.h"
#include "../incl/interpret.h"
#include <stdlib.h>


int main(int argc, char* argv[]) 
{
    InputStr input = {0};
    Lox lox = Lox_new();

    if (argc == 1)
    {
        return (Lox_repl(&lox) ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    const char* filename = argv[1];
    input = InputStr_from_file(filename);

    return (Lox_run(&lox, &input) ? EXIT_SUCCESS : EXIT_FAILURE);
}
