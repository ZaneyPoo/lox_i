#ifndef ZBASIC_PARSE_H_
#define ZBASIC_PARSE_H_
#include "lex.h"
#include <stdbool.h>


typedef enum ErrorState 
{
    OK = 0,
} ErrorState;


typedef struct Parser
{
    Lexer* lexer;
    Token* tokens;
    size_t num_tokens;
    size_t cursor;
    Token* current;
} Parser;

Parser Parser_new(Lexer* lexer);

void Parser_delete(Parser* parser);

bool Parser_check(Parser* parser, TokenType type);

bool Parser_check_peek(Parser* parser, TokenType type);

void Parser_next(Parser* parser);

bool Parser_parse(Parser* parser);

#endif // ZBASIC_PARSE_H_
