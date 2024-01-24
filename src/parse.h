#ifndef ZLOX_PARSE_H_PRIVATE_
#define ZLOX_PARSE_H_PRIVATE_
#include "../incl/parse.h"


static Node* Expression(Parser* parser);

static Node* Equality(Parser* parser);

static Node* Comparison(Parser* parser);

static Node* Term(Parser* parser);

static Node* Factor(Parser* parser);

static Node* Unary(Parser* parser);

static Node* Primary(Parser* parser);


#endif // ZLOX_PARSE_H_PRIVATE_
