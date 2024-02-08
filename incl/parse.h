#ifndef ZLOX_PARSE_H_
#define ZLOX_PARSE_H_
#include "lex.h"
#include <stdbool.h>

typedef struct Node Node;

typedef enum NodeType
{
    NodeExpr = 0,
    NodeExprEquality,
    NodeExprComparison,
    NodeExprTerm,
    NodeExprFactor,
    NodeExprUnary,
    NodeExprPrimaryLiteral,
    NodeExprPrimaryGrouping,
    NUM_NODE_TYPES,
    NodeParseError,
} NodeType;


// TODO: No clue if this is a good structure for this or not
struct Node 
{
    NodeType type;
    union 
    {
        // NodeExpr
        struct 
        {
            Node* data;
        };

        // NodeExprEquality
        // NodeExprComparison 
        // NodeExprTerm
        // NodeExprFactor
        struct 
        {
            Node* left;
            Token oper;
            Node* right;
        } NodeExprBinary;

        // NodeExprUnary
        struct 
        {
            Token oper;
            Node* right;
        } NodeExprUnary;


        // NodeExprPrimaryLiteral
        Token literal;

        // NodeExprPrimaryGrouping
        struct 
        {
            Token open_paren;
            Node* expr;
            Token close_paren;
        } NodeExprPrimaryGrouping;

        // NodeParseError
        // This is kinda hacky?
        struct 
        {
            Token token;
            const char* err_msg;
        } NodeParseError;
    };
};

Node* Node_new(NodeType type);

void Node_delete(Node* node);

const char* Node_get_type_name(NodeType type);

void Node_display(Node* node);


typedef struct Parser
{
    Lexer* lexer;
    Token current;
    Token prev;
    bool had_error;
    bool panic_mode;
} Parser;

void Parser_init(Parser* parser, Lexer* lexer);

void Parser_delete(Parser* parser);

Token* Parser_next(Parser* parser);

Token* Parser_peek(Parser* parser);

bool Parser_match(Parser* parser, TokenType type, ...);

void Parser_consume(Parser* parser, TokenType type, const char* message);

bool Parser_eof(Parser* parser);

Node* Parser_parse(Parser* parser);

void Parser_synchronize(Parser* parser);

#endif // ZLOX_PARSE_H_
