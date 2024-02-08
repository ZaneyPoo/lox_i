#include "../incl/interpret.h"
#include "../incl/utils.h"

#define MAX_REPL_CHARACTERS 1024

static void Lox_evaluate(Lox* lox);

Lox Lox_new(void)
{
    return (Lox){
        .AST = NULL,
        .parser = {0},
    };
}



void Lox_delete(Lox* lox)
{
    Node_delete(lox->AST);
    Parser_delete(&lox->parser);
}



bool Lox_run(Lox* lox, InputStr* input)
{
    Lexer lexer = Lexer_new(input);
    Parser_init(&lox->parser, &lexer);

    lox->AST = Parser_parse(&lox->parser);

    Lexer_delete(&lexer);
    Node_delete(lox->AST);

    return !lox->parser.had_error;
}



// TODO: Test this because this is probably doo doo
bool Lox_repl(Lox* lox)
{
    bool result = true;

    while (result)
    {
        InputStr input = InputStr_from_stdin(">> ", MAX_REPL_CHARACTERS);
        
        if (!input.text)
            return 0;

        result = Lox_run(lox, &input);
    }

    return result;
}



static void Lox_evaluate(Lox* lox)
{
}
