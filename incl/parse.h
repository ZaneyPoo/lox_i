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
    NodeExprPrimary,
    NodeExprPrimaryLiteral,
    NodeExprPrimaryGrouping,
    NUM_NODE_TYPES,
} NodeType;


// TODO: No clue if this is a good structure for this or not
struct Node 
{
    NodeType type;
    union 
    {
        struct 
        {
            Node* data;
        };

        struct 
        {
            Node* left;
            Token oper;
            Node* right;
        } NodeExpBinary;

        struct 
        {
            Token oper;
            Node* data;
        } NodeExprUnary;


        Token literal;

        struct 
        {
            Token open_paren;
            Node* expr;
            Token close_paren;
        } NodeExprPrimaryGrouping;
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
} Parser;

Parser Parser_new(Lexer* lexer);

void Parser_delete(Parser* parser);

Token* Parser_next(Parser* parser);

Token* Parser_peek(Parser* parser);

bool Parser_match(Parser* parser, TokenType type, ...);

bool Parser_eof(Parser* parser);

Node* Parser_parse(Parser* parser);

#endif // ZLOX_PARSE_H_
