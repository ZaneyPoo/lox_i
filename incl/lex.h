#ifndef ZLOX_LEX_H_
#define ZLOX_LEX_H_
#include "utils.h"
#include <stddef.h>
#include <stdbool.h>

typedef enum TokenType 
{
    // Minor tokens
    // "minor" tokens are tokens which are not keywords, literals,
    // or identifiers.
    TOKEN_OPEN_PAREN = 0,
    TOKEN_CLOSE_PAREN,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_FSLASH,
    TOKEN_STAR,

    TOKEN_EQ,
    TOKEN_BANG,
    TOKEN_GT,
    TOKEN_LT,
    TOKEN_EQ_EQ,
    TOKEN_BANG_EQ,
    TOKEN_GTE,
    TOKEN_LTE,

    // Major tokens ------------------------------

    // Symbols ----------
    TOKEN_IDENT,
    TOKEN_STR,
    TOKEN_NUM,

    // Keywords ---------
    TOKEN_NIL,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_AND,
    TOKEN_OR,

    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,

    TOKEN_FUN,
    TOKEN_RETURN,
    TOKEN_CLASS,
    TOKEN_THIS, 
    TOKEN_SUPER,

    TOKEN_VAR,
    TOKEN_PRINT,

    TOKEN_ERROR,
    TOKEN_EOF,
    TOKEN_NUM_TOKENS,
} TokenType;


typedef struct Token 
{
    TokenType type;
    const char* str;
    size_t len;
    size_t line;
} Token;

void Token_display(const Token* token);

const char* Token_get_type_name(TokenType type);

typedef struct Lexer 
{
    char* data;
    size_t data_len;
    size_t cursor;
    size_t line;
    char current;
} Lexer;

Lexer Lexer_new(InputStr* input);

void Lexer_delete(Lexer* lexer);

char Lexer_next(Lexer* lexer);

char Lexer_advance(Lexer* lexer, int num_times);

char Lexer_peek(Lexer* lexer);

bool Lexer_eof(Lexer* lexer);

void Lexer_skip_whitespace(Lexer* lexer);

void Lexer_skip_comment(Lexer* lexer);

Token Lexer_get_token(Lexer* lexer);

#endif // ZLOX_LEX_H_
