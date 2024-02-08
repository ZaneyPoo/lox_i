#ifndef ZLOX_INTERPRET_H_
#define ZLOX_INTERPRET_H_
#include "parse.h"

#define ZLOX_ERROR_LIMIT 20

typedef enum LoxErrType
{
    LOX_ERR_NONE = 0,
    LOX_ERR_PARSE,
} LoxErrType;

typedef struct Lox
{
    Node* AST;
    Parser parser;
} Lox;

Lox Lox_new(void);

void Lox_reset(Lox* interpreter);

void Lox_delete(Lox* interpreter);

bool Lox_run(Lox* interpreter, InputStr* input);

bool Lox_repl(Lox* interpreter);

#endif // ZLOX_INTERPRET_H_
