#include "../incl/file.h"
#include "../incl/lex.h"
#include "../incl/prompt.h"
#include <stdlib.h>



int main(int argc, char* argv[]) 
{
    const char* filename = "sample.basic";
    Lexer lexer;
    Token token;
    File input_file;

    // if (argc == 1)
    //     start_interative_mode();

    //filename = argv[1];
    input_file = File_new(filename);
    lexer = Lexer_new(input_file.text, input_file.len);

    do
    {
        token = Lexer_get_token(&lexer);
        Token_display(&token);
    } while (token.type != TOKEN_EOF);

    File_delete(&input_file);
    Lexer_delete(&lexer);
    return EXIT_SUCCESS;
}
