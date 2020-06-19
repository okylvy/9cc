#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "9cc.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is not correct.");
        return 1;

    }

    // tokenize and parse
    user_input = argv[1];
    token = tokenize(user_input);
    Node *node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // traverse the AST to emit assembly
    gen(node);

    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}
