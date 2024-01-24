#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

#define LIT_DEF(type) {type, #type}
static const struct 
{
    NodeType type;
    const char* display;
} LIT_NODES[] = {
    LIT_DEF(NodeExpr),
    LIT_DEF(NodeExprEquality),
    LIT_DEF(NodeExprComparison),
    LIT_DEF(NodeExprTerm),
    LIT_DEF(NodeExprFactor),
    LIT_DEF(NodeExprUnary),
    LIT_DEF(NodeExprPrimary),
    LIT_DEF(NodeExprPrimaryLiteral),
    LIT_DEF(NodeExprPrimaryGrouping),
    {NUM_NODE_TYPES, "__NUM_NODE_TYPES__"},
};
#undef LIT_DEF



Node* Node_new(NodeType type)
{
    Node* new_node = malloc(sizeof(Node));

    if (new_node)
        new_node->type = type;

    return new_node;
}



void Node_delete(Node* node)
{
    //TODO: Implement me once Node data structure is solidified
}



const char* Node_get_type_name(NodeType type)
{
    return LIT_NODES[type].display;
}



void Node_display(Node* node)
{
    printf("Type: %s\n", LIT_NODES[node->type].display);
}



Parser Parser_new(Lexer* lexer)
{
    return (Parser){lexer, {0}, {0}};
}



void Parser_delete(Parser* parser)
{
    

    Lexer_delete(parser->lexer);
    parser->lexer = NULL;
    parser->current = (Token){0};
    parser->prev = (Token){0};
}



Token* Parser_next(Parser* parser)
{
    if (Parser_eof(parser))
        return NULL;

    parser->prev = parser->current;
    parser->current = Lexer_get_token(parser->lexer);

    return &parser->current;
}



Token* Parser_peek(Parser* parser)
{
    return &parser->current;
}



bool Parser_eof(Parser* parser)
{
    return (parser->prev.type == TOKEN_EOF);
}



Node* Parser_parse(Parser* parser)
{
    return Expression(parser);
}



static Node* Expression(Parser* parser)
{
    return Equality(parser);
}



static Node* Equality(Parser* parser)
{
    Node* comparison = Comparison(parser);

    // TODO: Finish this

    return comparison;
}



static Node* Comparison(Parser* parser)
{
    Node* term = Term(parser);
    // TODO: Finish this
    return term;
}



static Node* Term(Parser* parser)
{
    Node* factor = Factor(parser);
    // TODO: Finish this

    return factor;
}



static Node* Factor(Parser* parser)
{
    Node* unary = Unary(parser);
    // TODO: Finish this
    return unary;
}



static Node* Unary(Parser* parser)
{
    if (Parser_peek(parser)->type == TOKEN_BANG 
        || Parser_peek(parser)->type == TOKEN_MINUS)
        return Unary(parser);

    return Primary(parser);
}



static Node* Primary(Parser* parser)
{
    switch (Parser_peek(parser)->type)
    {
        case TOKEN_NUM:
        case TOKEN_STR:
        case TOKEN_TRUE:
        case TOKEN_FALSE:
        case TOKEN_NIL:
            {
                Node* lit = Node_new(NodeExprPrimaryLiteral);
                lit->literal = parser->current;
                return lit;
            }

        case TOKEN_OPEN_PAREN:
            return Expression(parser);

        default:
            // TODO: trigger error state
            return NULL;
    }
}



