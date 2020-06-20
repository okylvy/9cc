#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//** tokenize.c **//

// token type
typedef enum {
    TK_RESERVED,  // symbol
    TK_IDENT,     // identifier
    TK_NUM,       // integer
    TK_EOF,       // end of input
} TokenKind;

typedef struct Token Token;
struct Token {
    TokenKind kind;  // token type
    Token *next;     // next input token
    int val;         // a value in case that kind is TK_NUM
    char *str;       // token string
    int len;         // token length
};

// the currenct token
Token *token;

// input program
char *user_input;

// function prototype
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool startswith(char *p, char *q);
Token *tokenize();


//** parse.c **//

// AST node type
typedef enum {
    ND_ADD,    // +
    ND_SUB,    // -
    ND_MUL,    // *
    ND_DIV,    // /
    ND_EQ,     // ==
    ND_NE,     // !=
    ND_LT,     // <
    ND_LE,     // <=
    ND_ASSIGN, // =
    ND_LVAR,   // Local value
    ND_NUM,    // Integer
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;    // NodeKind = ND_NUM
    int offset; // NodeKind = ND_LVAR
};

// function prototype
//Node *new_node(NodeKind kind);
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();


//** codegen.c **//
void gen(Node *node);
