#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hicc.h"

// This global array is to store the node which is processed by the parser.
Node *code[100];


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is not correct.");
        return 1;

    }

    // tokenize and parse
    user_input = argv[1];
    token = tokenize(user_input);

    // The bridge between tokenizer and parser is consume().
    Node *node = expr();
//TODO: Modify parse.c and call program() (maybe..)
//    Node *node = stmt();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // traverse the AST to emit assembly
    gen(node);

    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}
