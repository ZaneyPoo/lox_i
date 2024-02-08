#include "../incl/parse.h"
#include "../incl/utils.h"
#include <assert.h>
#include <stdarg.h>
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
    LIT_DEF(NodeExprPrimaryLiteral),
    LIT_DEF(NodeExprPrimaryGrouping),
    {NUM_NODE_TYPES, "__NUM_NODE_TYPES__"},
    LIT_DEF(NodeParseError),
};
#undef LIT_DEF


static Node* Expression(Parser* parser);

static Node* Equality(Parser* parser);

static Node* Comparison(Parser* parser);

static Node* Term(Parser* parser);

static Node* Factor(Parser* parser);

static Node* Unary(Parser* parser);

static Node* Primary(Parser* parser);

static void print_error(Parser* parser, Token* token, const char* msg);

// TODO: Implement this variadic version when ready
// Node* Node_new(NodeType type, void* args, ...)
Node* Node_new(NodeType type) 
{
    Node* new_node = malloc(sizeof(Node));

    if (new_node)
        new_node->type = type;

    return new_node;
}



// TODO: Does this cause stack overflow????
void Node_delete(Node* node)
{
    if (!node)
        return;

    switch (node->type)
    {
        case NodeExpr:
            Node_delete(node->data);
            break;

        case NodeExprEquality:
            [[fallthrough]];
        case NodeExprComparison:
            [[fallthrough]];
        case NodeExprTerm:
            [[fallthrough]];
        case NodeExprFactor:
            Node_delete(node->NodeExprBinary.left);
            Node_delete(node->NodeExprBinary.right);
            break;

        case NodeExprUnary:
            Node_delete(node->NodeExprUnary.right);
            break;

        case NodeExprPrimaryGrouping:
            Node_delete(node->NodeExprPrimaryGrouping.expr);
            break;

        case NodeExprPrimaryLiteral:
            [[fallthrough]];
        case NUM_NODE_TYPES:
            [[fallthrough]];
        case NodeParseError:
            break;

    }

    free(node);
    node = NULL;
}



const char* Node_get_type_name(NodeType type)
{
    return LIT_NODES[type].display;
}



void Node_display(Node* node)
{
    printf("Type: %s\n", LIT_NODES[node->type].display);
}



void Parser_init(Parser* parser, Lexer* lexer)
{
    assert(parser);
    *parser = (Parser){
        .lexer = lexer,
        .current = {0},
        .prev = {0},
        .had_error = false,
        .panic_mode = false,
    };
}



void Parser_delete(Parser* parser)
{
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



#define MATCH(parser, type, ...) Parser_match(parser, type, __VA_ARGS__, TOKEN_EOF)
bool Parser_match(Parser* parser, TokenType type, ...) 
{
    va_list args;
    va_start(args, type);

    while (type != TOKEN_EOF)
    {
        if (Parser_peek(parser)->type == type)
        {
            Parser_next(parser);
            va_end(args);
            return true;
        }

        type = va_arg(args, TokenType);
    }

    va_end(args);
    return false;
}



void Parser_consume(Parser* parser, TokenType type, const char* message)
{
    if (parser->current.type == type)
    {
        Parser_next(parser);
        return;
    }

    print_error(parser, &parser->current, message);
}



bool Parser_eof(Parser* parser)
{
    return (parser->prev.type == TOKEN_EOF);
}



Node* Parser_parse(Parser* parser)
{
    Node* expression = Expression(parser);

    if (parser->had_error)
        return NULL;

    return expression;
}



void Parser_synchronize(Parser* parser)
{
    Parser_next(parser);

    while (!Parser_eof(parser))
    {
        if (parser->prev.type == TOKEN_SEMICOLON)
        {
            parser->panic_mode = false;
            return;
        }

        switch (Parser_peek(parser)->type)
        {
            case TOKEN_CLASS:
                [[fallthrough]];
            case TOKEN_FUN:
                [[fallthrough]];
            case TOKEN_VAR:
                [[fallthrough]];
            case TOKEN_FOR:
                [[fallthrough]];
            case TOKEN_IF:
                [[fallthrough]];
            case TOKEN_WHILE:
                [[fallthrough]];
            case TOKEN_PRINT:
                [[fallthrough]];
            case TOKEN_RETURN:
                parser->panic_mode = false;
                return;

            default:
                Parser_next(parser);
                break;
        }
    }
}



static Node* Expression(Parser* parser)
{
    return Equality(parser);
}



static Node* Equality(Parser* parser)
{
    Node* equality = Comparison(parser);

    while (MATCH(parser, TOKEN_BANG, TOKEN_EQ)) 
    {
        Node* lhs = equality;
        Token operator = parser->prev;
        Node* rhs = Comparison(parser);

        equality = Node_new(NodeExprComparison);
        equality->NodeExprBinary.left = lhs;
        equality->NodeExprBinary.oper = operator;
        equality->NodeExprBinary.right = rhs;
    }

    return equality;
}



static Node* Comparison(Parser* parser)
{
    Node* comparison = Term(parser);
    
    while (MATCH(parser, TOKEN_GT, TOKEN_GTE, TOKEN_LT, TOKEN_LTE))
    {
        Node* lhs = comparison;
        Token operator = parser->prev;
        Node* rhs = Term(parser);

        comparison = Node_new(NodeExprComparison);
        comparison->NodeExprBinary.left = lhs;
        comparison->NodeExprBinary.oper = operator;
        comparison->NodeExprBinary.right = rhs;
    }

    return comparison;
}



static Node* Term(Parser* parser)
{
    Node* term = Factor(parser);

    while (MATCH(parser, TOKEN_MINUS, TOKEN_PLUS))
    {
        Node* lhs = term;
        Token operator = parser->prev;
        Node* rhs = Factor(parser);

        term = Node_new(NodeExprTerm);
        term->NodeExprBinary.left = lhs;
        term->NodeExprBinary.oper = operator;
        term->NodeExprBinary.right = rhs;
    }

    return term;
}



static Node* Factor(Parser* parser)
{
    Node* factor = Unary(parser);

    while (MATCH(parser, TOKEN_FSLASH, TOKEN_STAR))
    {
        Node* lhs = factor;
        Token operator = parser->prev;
        Node* rhs = Unary(parser);

        factor = Node_new(NodeExprFactor);
        factor->NodeExprBinary.left = lhs;
        factor->NodeExprBinary.oper = operator;
        factor->NodeExprBinary.right = rhs;
    }

    return factor;
}



static Node* Unary(Parser* parser)
{
    if (MATCH(parser, TOKEN_BANG, TOKEN_MINUS))
    {
        Token operator = parser->prev;
        Node* right = Unary(parser);

        Node* unary = Node_new(NodeExprUnary);
        unary->NodeExprUnary.oper = operator;
        unary->NodeExprUnary.right = right;

        return unary;
    }

    return Primary(parser);
}



static Node* Primary(Parser* parser)
{
    Node* primary = NULL;

    switch (Parser_peek(parser)->type)
    {
        case TOKEN_NUM:
            [[fallthrough]];
        case TOKEN_STR:
            [[fallthrough]];
        case TOKEN_TRUE:
            [[fallthrough]];
        case TOKEN_FALSE:
            [[fallthrough]];
        case TOKEN_NIL:
            {
                primary = Node_new(NodeExprPrimaryLiteral);
                primary->literal = parser->current;
            }
            break;

        case TOKEN_OPEN_PAREN:
            {
                Parser_next(parser);
                Token open_paren = parser->prev;
                Node* expr = Expression(parser);

                Parser_consume(parser, TOKEN_CLOSE_PAREN, "Expected ')' after expression");
                Token close_paren = parser->prev;
                primary = Node_new(NodeExprPrimaryGrouping);
                primary->NodeExprPrimaryGrouping.open_paren = open_paren;
                primary->NodeExprPrimaryGrouping.expr = expr;
                primary->NodeExprPrimaryGrouping.close_paren = close_paren;
            }
            break;

        default:
            // TODO: trigger error state
            break;
    }

    Parser_next(parser);
    return primary;
}



static void print_error(Parser* parser, Token* token, const char* msg)
{
    if (parser->panic_mode)
        return;

    parser->panic_mode = false;
    LOG_INFO("Parser: entering PANIC MODE...");
    fprintf(stderr, "[line %zu] Error:", token->line);
    
    switch (token->type)
    {
        case TOKEN_EOF:
            fprintf(stderr, " at EOF");
            break;

        case TOKEN_ERROR:
            break;

        default:
            fprintf(stderr, " at '%.*s'", (int)token->len, token->str);
            break;
    }

    fprintf(stderr, ": %s\n", msg);
    parser->had_error = true;
    LOG_INFO("Parser: setting error flag to true..."); 
}

#undef MATCH
