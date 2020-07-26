#include "hicc.h"

/**
 * Return 1 if the argument is alphabet or '_'.
 */
int is_alpha(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') ||
           (c == '_');
}

/**
 * Return 1 if the argument is alphabet, number, or '_'.
 */
int is_alnum(char c) {
    return is_alpha(c) ||
           ('0' <= c && c <= '9');
}

/**
 * Return 1 if the argument is alphabet, number, or '_'.
 */
static char *starts_with_reserved_word(char *p) {
    static char *kw[] = {"return", "if", "else", "while", "for"};

    for (int i=0; i<sizeof(kw) / sizeof(*kw); i++) {
        int len = strlen(kw[i]);
        if (startswith(p, kw[i]) && !is_alnum(p[len])) {
            return kw[i];
        }
    }
}

/**
 * 
 */
bool startswith(char *p, char *q) {
    return memcmp(p, q, strlen(q)) == 0;
}

/**
 * Store the current token information.
 */
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

/**
 * 
 */
Token *tokenize(char *user_input) {
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // Skip spaces.
        if (isspace(*p)) {
            p++;
            continue;
        }

        // Multi-letter punctuator
        if (startswith(p, "==") ||
            startswith(p, "!=") ||
            startswith(p, "<=") ||
            startswith(p, ">=")) {
                cur = new_token(TK_RESERVED, cur, p, 2);
                p += 2;
                continue;
            }

        // Single-letter punctuator
        if (strchr("+-*/()<>=;", *p)) {
            cur = new_token(TK_RESERVED, cur, p, 1);
            p++;
            continue;
        }

        // return
        if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        }

        // Integer literal
        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        // Identifier
        if (is_alpha(*p) || (*p & 0x80)) {
            char *q = p++;
            while (is_alnum(*p) || (*p & 0x80)) {
                p++;
            }
            cur = new_token(TK_IDENT, cur, q, p-q);
            continue;
        }

        error_at(p, "Can't tokenize.");
    }
    new_token(TK_EOF, cur, p, 0);

    return head.next;
}
