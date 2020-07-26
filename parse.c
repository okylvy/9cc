#include "hicc.h"

Token *token;  // The current token under processing.
LVar *locals;  // To store local variables defined in user's program.

/**
 * 
 */
bool at_eof() {
    return token->kind == TK_EOF;
}

/**
 * Return true if next token is expected value.
 * Otherwise, return false.
 */
bool equal(Token *tok, char *op) {
    return strlen(op) == tok->len && !strncmp(tok->str, op, tok->len);
}

/**
 * Return true and proceed a token if next token is expected value.
 * Otherwise, return false.
 */
// TODO: Currently, this function is equal to consume().
//  "return false" should be error.
Token *skip(char *op) {
    if (equal(token, op)) {
        token = token->next;
        return token;
    }
    return false;
}

/**
 * Return true and proceed a token if next token is expected value.
 * Otherwise, return false.
 */
bool consume(char *op) {
    if (equal(token, op)) {
        token = token->next;
        return true;
    }
    return false;
}

/**
 * 
 */
Token *consume_ident(void) {
    if (token->kind != TK_IDENT)
        return NULL;
    Token *t = token;
    token = token->next;
    return t;
}

/**
 * 
 */
bool consume_return() {
    if (token->kind != TK_RETURN) {
        return false;
    }
    token = token->next;
    return true;
}

/**
 * 
 */
Node *new_node(NodeKind kind, Token *tok) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->token = tok;
    return node;
}

/**
 * 
 */
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs, Token *tok) {
    Node *node = new_node(kind, tok);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

/**
 * 
 */
Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

/**
 * 
 */
void expect(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
            error_at(token->str, "expected \"%s\"", op);
    token = token->next;
}

/**
 * If next token is a number, proceed the token and return the value.
 * Otherwise, report the error.
 */
int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "expected a number.");
    int val = token->val;
    token = token->next;
    return val;
}

/**
 * Search a variable by name, and return the variable if it's found.
 * Otherwise, return NULL
 */
LVar *find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next) {
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len)) {
            return var;
        }
    }
    return NULL;
}

/**
 * 
 */
Node *program(Token *token_arg) {
    token = token_arg;

    Node head;
    head.next = NULL;
    Node *cur = &head;
    while (!at_eof()) {
        cur->next = stmt();
        cur = cur->next;
    }

    return head.next;
}

/**
 * 
 */
Node *stmt() {
    Node *node;

    // Control structure.
    // if statement.
    if (consume("if")) {
        Node *node = new_node(ND_IF, token);
        skip("(");
        node->cond = expr();
        skip(")");
        node->then = stmt();
        if (consume("else")) {
            node->els = stmt();
        }
        return node;
    }

    // return statement.
    if (consume_return()) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr();
        skip(";");
        return node;
    }

    node = expr();
    skip(";");

    return node;
}

/**
 * 
 */
Node *expr() {
    return assign();
}

/**
 * 
 */
Node *assign() {
    Node *node = equality();
    if (consume("="))
        node = new_binary(ND_ASSIGN, node, assign(), token);
    return node;
}

/**
 * 
 */
Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("=="))
            node = new_binary(ND_EQ, node, relational(), token);
        else if (consume("!="))
            node = new_binary(ND_NE, node, relational(), token);
        else
            return node;
    }
}

/**
 * 
 */
Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<"))
            node = new_binary(ND_LT, node, add(), token);
        else if (consume("<="))
            node = new_binary(ND_LE, node, add(), token);
        else if (consume(">"))
            node = new_binary(ND_LT, add(), node, token);
        else if (consume(">="))
            node = new_binary(ND_LE, add(), node, token);
        else
            return node;
    }
 }

/**
 * 
 */
Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+"))
            node = new_binary(ND_ADD, node, mul(), token);
        else if (consume("-"))
            node = new_binary(ND_SUB, node, mul(), token);
        else
            return node;
    }
}

/**
 * 
 */
Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*"))
            node = new_binary(ND_MUL, node, unary(), token);
        else if (consume("/"))
            node = new_binary(ND_DIV, node, unary(), token);
        else
            return node;
    }
}

/**
 * 
 */
Node *unary() {
    if (consume("+"))
        return unary();
    if (consume("-"))
        return new_binary(ND_SUB, new_node_num(0), unary(), token);
    return primary();
}

/**
 * 
 */
Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = find_lvar(tok);
        if (lvar) {
            node->offset = lvar->offset;
        } else {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;
            lvar->offset = locals==NULL ? 0 : locals->offset + 8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
    }

    return new_node_num(expect_number());
}
