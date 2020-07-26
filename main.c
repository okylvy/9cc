#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hicc.h"

char *user_input;  // Input program.

/**
 * Points out the location where an error happens.
 */
void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

/**
 * Main function.
 */
int main(int argc, char **argv) {
    // Check arguments passed by user.
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is not correct.");
        return 1;
    }

    // Tokenize and parse.
    user_input = argv[1];
    Token *token = tokenize(user_input);

    // The bridge between tokenizer and parser is consume().
    Node *node = program(token);

    // Traverse the AST to emit assembly.
    codegen(node);

    return 0;
}
