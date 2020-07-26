#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * main.c
 */

// Function prototype
void error_at(char *loc, char *fmt, ...);


/**
 * tokenize.c
 */

// Token type
typedef enum {
    TK_RESERVED,  // Symbol
    TK_IDENT,     // Identifier
    TK_NUM,       // Integer
    TK_RETURN,    // return
    TK_EOF,       // End of input
} TokenKind;

// Contents of Token
typedef struct Token Token;
struct Token {
    TokenKind kind;  // Token type
    Token *next;     // Next input token
    int val;         // A value in case that kind is TK_NUM
    char *str;       // Token string
    int len;         // Token length
};

// Type of local valuable
typedef struct LVar LVar;
struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

// Function prototype
int is_alpha(char c);
int is_alnum(char c);
void expect(char *op);
int expect_number();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
static char *starts_with_reserved_word(char *p);
bool startswith(char *p, char *q);
Token *tokenize();


/**
 * parse.c
 */

// AST node type
typedef enum {
    ND_ADD,     // +
    ND_SUB,     // -
    ND_MUL,     // *
    ND_DIV,     // /
    ND_EQ,      // ==
    ND_NE,      // !=
    ND_LT,      // <
    ND_LE,      // <=
    ND_ASSIGN,  // =
    ND_LVAR,    // Local value
    ND_NUM,     // Integer
    ND_RETURN,  // return
    ND_IF,      // if
} NodeKind;

// Contents of Node
typedef struct Node Node;
struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    Node *next;
    Token *token;
    int val;       // NodeKind = ND_NUM
    int offset;    // NodeKind = ND_LVAR
    /** if statement **/
    Node *cond;    // condition
    Node *then;    // body
    Node *els;     // else
};

// Function prototype
bool at_eof();
bool equal(Token *tok, char *op);
Token *skip(char *op);
bool consume(char *op);
Token *consume_ident(void);
bool consume_return();
Node *new_node(NodeKind kind, Token *tok);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs, Token *tok);
Node *new_node_num(int val);
LVar *find_lvar(Token *tok);
Node *program(Token *token);
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();


/**
 * codegen.c
 */

void codegen(Node *node);
void gen_lval(Node *node);
void gen(Node *node);
