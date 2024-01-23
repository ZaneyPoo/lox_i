#include "../incl/lex.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEAD_CODE fprintf(stderr, "DEAD CODE FIX ME")

//static const char TOKEN_FORMAT_SPEC[] = "%s(%.*s)";
#define TOKEN_FORMAT_SPEC "%s(\"%.*s\")"

static const char MINOR_TOKENS[] = "(){},.-+;/*=!><";
static const char END_MARKER = '\0';

#define LIT_DEF(type, pattern) {TOKEN_##type, pattern, sizeof(pattern) - 1, #type}
const struct 
{
    TokenType type; 
    const char* pattern; 
    size_t pattern_size;
    const char* display;
} LIT_TOKENS[] = {
    LIT_DEF(OPEN_PAREN, "("),
    LIT_DEF(CLOSE_PAREN, ")"),
    LIT_DEF(OPEN_BRACE, "{"),
    LIT_DEF(CLOSE_BRACE, "}"),
    LIT_DEF(COMMA, ","),
    LIT_DEF(DOT, "."),
    LIT_DEF(MINUS, "-"),
    LIT_DEF(PLUS, "+"),
    LIT_DEF(SEMICOLON, ";"),
    LIT_DEF(FSLASH, "/"),
    LIT_DEF(STAR, "*"),

    LIT_DEF(EQ, "="), 
    LIT_DEF(BANG, "!"),
    LIT_DEF(GT, ">"), 
    LIT_DEF(LT, "<"), 

    // Double char operators
    LIT_DEF(EQ_EQ, "=="),
    LIT_DEF(BANG_EQ, "!="),
    LIT_DEF(GTE, ">="),
    LIT_DEF(LTE, "<="), 

    LIT_DEF(IDENT, "[a-zA-Z_][a-zA-Z_0-9]*"),
    LIT_DEF(STR, "(\".[^\"]*\")|(\'.[^\']*\')"),
    LIT_DEF(NUM, "[0-9]+(\\.[0-9]*)?"),

    LIT_DEF(NIL, "nil"), 
    LIT_DEF(TRUE, "true"),
    LIT_DEF(FALSE, "false"),
    LIT_DEF(AND, "and"),
    LIT_DEF(OR, "or"),
    LIT_DEF(IF, "if"),
    LIT_DEF(ELSE, "else"),
    LIT_DEF(WHILE, "while"),
    LIT_DEF(FOR, "for"),
    LIT_DEF(FUN, "fun"),
    LIT_DEF(RETURN, "return"),
    LIT_DEF(CLASS, "class"),
    LIT_DEF(THIS, "this"),
    LIT_DEF(SUPER, "super"),
    LIT_DEF(VAR, "var"),
    LIT_DEF(PRINT, "print"),
    LIT_DEF(ERROR, "__ERROR__"),
    LIT_DEF(EOF, "\0"),
    {TOKEN_NUM_TOKENS, "__NUM_TOKENS__", 15, "SOMETHING'S WRONG"},
};
#undef LIT_DEF



static inline bool is_minor(char ch)
{
    char* found = strchr(MINOR_TOKENS, ch);
    return (found && *found != '\0');
}



static inline bool is_2char_minor(TokenType type)
{
    switch (type)
    {
        case TOKEN_EQ_EQ:
            [[fallthrough]];
        case TOKEN_BANG_EQ:
            [[fallthrough]];
        case TOKEN_GTE:
            [[fallthrough]];
        case TOKEN_LTE:
            return true;

        default:
            return false;
    }
}



static inline bool is_string(char current)
{
    return (current == '\"' || current == '\'');
}



static inline bool is_ident_start(char ch)
{
    return (isalpha(ch) || ch == '_');
}



static inline bool is_ident_mid(char ch)
{
    return (isalnum(ch) || ch == '_');
}



static inline TokenType get_keyword(Token* token)
{
    bool match = false;
    TokenType type = TOKEN_IDENT;

    for (size_t i = TOKEN_NIL; !match && i < TOKEN_ERROR; ++i)
    {

        if (LIT_TOKENS[i].pattern_size == token->len
            && !strncmp(token->str, LIT_TOKENS[i].pattern, token->len))
        {
            type = i;
            match = true; 
        }
    }

    return type;
}



void Token_display(const Token* token)
{
    const char* format = NULL;

    if (!token)
        return;

    // TODO: clean this up. feels kinda pointless
    switch (token->type)
    {
        case TOKEN_NUM:
            [[fallthrough]];
        case TOKEN_STR:
            format = "%s(%.*s)\n";
            break;

        case TOKEN_ERROR:
            [[fallthrough]];
        case TOKEN_IDENT:
            format = "%s(\"%.*s\")\n";
            break;

        case TOKEN_EOF:
            format = "%s\n";
            break;

        default:
            format = "%s(\'%.*s\')\n";
            break;
    }

    printf(format, LIT_TOKENS[token->type].display, (int)token->len, token->str);
}



const char* Token_get_type_name(TokenType type)
{
    const char* result = LIT_TOKENS[type].display;

    if (!result)
        return "Unknown TokenType";

    return result;
}



static inline bool consume_string(Lexer* lexer, Token* token)
{
    char current = lexer->current;
    char quote = current;

    // TODO: add support for escaping quotes
    token->type = TOKEN_STR;

    do
    {
        current = Lexer_next(lexer);
        ++token->len;

        // TODO: probs get rid of this. I don't like multiline strings
        if (current == '\n')
            ++lexer->line;
    } while (current != quote && !Lexer_eof(lexer));

    if (Lexer_eof(lexer))
    {
        token->type = TOKEN_ERROR;
        return false;
    }

    // trim off begin and end quotes
    ++token->str;
    token->len -= 2;

    Lexer_next(lexer);
    return true;
}



static inline bool consume_number(Lexer* lexer, Token* token)
{
    // TODO: add error conditions for this
    char current = lexer->current;
    token->type = TOKEN_NUM;

    do
    {
        current = Lexer_next(lexer);
        if (isdigit(current)) 
            ++token->len;
    } while (isdigit(current) && !Lexer_eof(lexer));

    if (current == '.') // Float
    {
        do 
        {
            current = Lexer_next(lexer);
            if (isdigit(current)) 
                ++token->len;
        } while (isdigit(current) && !Lexer_eof(lexer));
    }

    Lexer_next(lexer);
    return true;
}



static inline bool consume_ident(Lexer* lexer, Token* token)
{
    // TODO: add error conditions for this
    char current = lexer->current;

    do 
    {
        current = Lexer_next(lexer);
        if (is_ident_mid(current)) 
            ++token->len;
    } while (is_ident_mid(current) && !Lexer_eof(lexer));

    token->type = get_keyword(token);

    Lexer_next(lexer);
    return true;
}



Lexer Lexer_new(const char* data, size_t len)
{
    char* str = malloc(sizeof(char) * len);

    // TODO: think about removing this for a failure return instead
    if (!str)
    {
        fprintf(stderr, "Lexer error: allocation request of %zu bytes failed\n", len);
        exit(EXIT_FAILURE);
    }

    strncpy(str, data, len);

    return (Lexer){str, len, 0, 0, *str};
}



void Lexer_delete(Lexer* lexer)
{
    if (lexer->data) free(lexer->data);
    lexer->data = NULL;
    lexer->data_len = lexer->cursor = 0;
    lexer->current = '\0';
}




char Lexer_next(Lexer* lexer)
{
    ++lexer->cursor;

    if (lexer->cursor >= lexer->data_len)
        lexer->current = END_MARKER;
    else 
        lexer->current = lexer->data[lexer->cursor];

    return lexer->current;
}



char Lexer_peek(Lexer* lexer)
{
    if ((lexer->cursor + 1) >= lexer->data_len)
        return END_MARKER;

    return lexer->data[lexer->cursor + 1];
}



char Lexer_advance(Lexer* lexer, int num_times)
{
    lexer->cursor += num_times;

    if (lexer->cursor >= lexer->data_len)
        lexer->current = END_MARKER;
    else
        lexer->current = lexer->data[lexer->cursor];

    return lexer->current;
}



bool Lexer_eof(Lexer* lexer)
{
    //return (lexer->cursor >= lexer->data_len);
    return (lexer->current == END_MARKER);
}



void Lexer_skip_whitespace(Lexer* lexer)
{
    bool is_whitespace = false;

    do
    {
        char ch = lexer->current;

        switch (ch)
        {
            case '\n':
                ++lexer->line;
                [[fallthrough]];
            case ' ':
                [[fallthrough]];
            case '\t':
                [[fallthrough]];
            case '\v':
                [[fallthrough]];
            case '\f':
                [[fallthrough]];
            case '\r':
                Lexer_next(lexer);
                break;

            default:
                is_whitespace = false;
        }
    } while (is_whitespace);
}



void Lexer_skip_comment(Lexer* lexer)
{
    if (lexer->current == '/'
        && Lexer_peek(lexer) == '/')
    {
        while (Lexer_next(lexer) != '\n');

        ++lexer->line;
        Lexer_next(lexer);
    }
}



static TokenType get_minor_token_type(char current, char next)
{
    bool dbl_operator = (next == '=');

    switch (current)
    {

        case '(':
            return TOKEN_OPEN_PAREN;

        case ')':
            return TOKEN_CLOSE_PAREN;

        case '{':
            return TOKEN_OPEN_BRACE;

        case '}':
            return TOKEN_CLOSE_BRACE;

        case ',':
            return TOKEN_COMMA;

        case '.':
            return TOKEN_DOT;

        case '-':
            return TOKEN_MINUS;

        case '+':
            return TOKEN_PLUS;

        case ';':
            return TOKEN_SEMICOLON;

        case '*':
            return TOKEN_STAR;

        case '/':
            return TOKEN_FSLASH;

        case '=':
            return (dbl_operator) ? TOKEN_EQ_EQ : TOKEN_EQ;

        case '!':
            return (dbl_operator) ? TOKEN_BANG_EQ : TOKEN_BANG;

        case '<':
            return (dbl_operator) ? TOKEN_LTE : TOKEN_LT;

        case '>':
            return (dbl_operator) ? TOKEN_GTE : TOKEN_GT;

        default:
            return TOKEN_ERROR;
    }
}
#undef CASE


Token Lexer_get_token(Lexer* lexer)
{
    Lexer_skip_whitespace(lexer);
    Lexer_skip_comment(lexer);

    Token token = {
        .str = (lexer->data + lexer->cursor),
        .len = 1, 
        .line = lexer->line
    };

    char current = lexer->current;

    if (Lexer_eof(lexer))
        return (Token){TOKEN_EOF, NULL, 1, lexer->line};


    if (is_minor(current))
    {
        token.type = get_minor_token_type(current, Lexer_peek(lexer));

        if (is_2char_minor(token.type))
            ++token.len;

        Lexer_advance(lexer, token.len);

        return token;
    }


    if (is_string(current))
    {
        // TODO: handle error value when string is unterminated
        consume_string(lexer, &token);

        return token;
    }


    if (isdigit(current))
    {
        // TODO: error handle
        consume_number(lexer, &token);

        return token;
    }


    if (is_ident_start(current))
    {
        // TODO: error handle
        consume_ident(lexer, &token);

        return token;
    }

    return (Token){TOKEN_ERROR, NULL, 1, lexer->line};
}
