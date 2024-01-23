#include "../lib/unity.h"
#include "../incl/lex.h"
#include <stdio.h>
#include <string.h>

#define LOCAL_BUF_SIZE 1024
#define TOKEN_ERR_FORMAT_SPEC "Expected: %s, Got: %s"

#define ID TOKEN_IDENT
#define NUM TOKEN_NUM 
#define STR TOKEN_STR

#define TOKEN_TEST(test_str) \
    token_test(test_str, sizeof(test_str), test_str##_REF, (sizeof(test_str##_REF) / sizeof(TokenType)))

static const char MINORS[] = "() {} , . - + ; / * = ! > < == != >= <=";
static const TokenType MINORS_REF[] = {
    TOKEN_OPEN_PAREN,
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
    TOKEN_EOF,
};

static const char MINORS_NO_SPACE[] = "(){},.-+;/*=!><";
static const TokenType MINORS_NO_SPACE_REF[] = {
    TOKEN_OPEN_PAREN,
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
    TOKEN_EOF,
};

static const char IDENTIFIERS[] = "the quick brown fox";
static const TokenType IDENTIFIERS_REF[] = {
    ID, ID, ID, ID, TOKEN_EOF
};

static const char STRING[] = "First \"double quote string\" Second \'single quote string\'";
static const TokenType STRING_REF[] = {
    ID, STR, ID, STR, TOKEN_EOF
};

static const char NUMBERS[] = "1 12 123 1234 012345 1.0 1.234 123.456";
static const TokenType NUMBERS_REF[] = {
    NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM, TOKEN_EOF
};


static const char KEYWORDS[] = "nil true false and or if else while for fun return class this super var print";
static const TokenType KEYWORDS_REF[] = {
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
    TOKEN_EOF,
};

static inline size_t min(size_t a, size_t b)
{
    return (a <= b ? a : b);
}


void setUp(void) 
{
}



void tearDown(void)
{
}



void get_tokens(Lexer* lexer, Token* target, size_t max)
{
    for (size_t i = 0; i < max; ++i)
    {
        target[i] = Lexer_get_token(lexer);
    }
}



void check_tokens(const Token* tokens, size_t tokens_len, const TokenType* ref, size_t ref_len)
{
    size_t len = min(tokens_len, ref_len);
    char error_msg[LOCAL_BUF_SIZE];


    for (size_t i = 0; i < len; ++i)
    {
        snprintf(error_msg, 
                LOCAL_BUF_SIZE, 
                "Expected: %s, Got: %s",
                Token_get_type_name(ref[i]),
                Token_get_type_name(tokens[i].type));
        Token_display(tokens + i);
        TEST_ASSERT_MESSAGE((tokens[i].type == ref[i]), error_msg);
    }
}



void token_test(const char* str, size_t str_size, const TokenType* ref, size_t ref_size)
{

    Lexer lexer = Lexer_new(str, str_size);
    Token tokens[LOCAL_BUF_SIZE] = {0};

    get_tokens(&lexer, tokens, LOCAL_BUF_SIZE);
    check_tokens(tokens, LOCAL_BUF_SIZE, ref, ref_size);

    Lexer_delete(&lexer);
}



void test_minors(void) 
{
    TOKEN_TEST(MINORS);
}



void test_minors_no_space(void)
{
    TOKEN_TEST(MINORS_NO_SPACE);
}



void test_identifiers(void)
{
    TOKEN_TEST(IDENTIFIERS);
}



void test_string(void)
{
    TOKEN_TEST(STRING);
}



void test_numbers(void)
{
    TOKEN_TEST(NUMBERS);
}



void test_keywords(void)
{
    TOKEN_TEST(KEYWORDS);
}



int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_minors);
    RUN_TEST(test_minors_no_space);
    RUN_TEST(test_identifiers);
    RUN_TEST(test_string);
    RUN_TEST(test_numbers);
    RUN_TEST(test_keywords);

    return UNITY_END();
}
