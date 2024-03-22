/* TODO: c(lear), 3d6 without + or - performs a clear operation.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utilities.h"
#include "parser.h"
#include "diceio.h"

#define BUFFER_SIZE 10000

int get_number(char* s, char **pos) {
    *pos = strchr(s, ':');
    if (*pos == NULL) {
        *pos = s;
        return 1;
    }
    int state = 1;
    int num = 0;
    for (; s < *pos; s++) {
        if (state == 1) {
            if (*s >= '0' && *s <= '9') {
                num = num * 10 + (*s - '0');
                state = 2;
            } else if (*s == ' ' || *s == '\t'){
            } else{
                return 0;
            }
        } else if (state == 2) {
            if (*s >= '0' && *s <= '9') {
                num = num * 10 + (*s - '0');
                state = 2;
            } else if (*s == ' ' || *s == '\t'){
                state = 3;
            } else {
                return 0;
            }
        } else if (state == 3) {
            if (*s != ' ' && *s != '\t'){
                return 0;
            }
        }
    }
    *pos = *pos + 1;
    // while (**pos == ' ' || **pos == '\t')
    //     *pos = *pos + 1;
    return num;
}

void dialog(flags f) {

    char raw[BUFFER_SIZE];
    int total = 0;
    int state = ADD;

    if (f.verbose)
        puts("Enter some dice strings!");

    while (true) { 
        if (f.verbose)
            printf(">>> ");
        char *result = fgets(raw, BUFFER_SIZE, stdin);
        if (result == NULL || raw[0] == 'q' || raw[0] == 'Q')
            return;
        total = process_command_line(raw, f, total, &state);
    } //while (true)
}

int process_command_line(char *raw, flags f, int total, int *state) {
    char *input, *rewind;
    bool error = false;
    remove_whitespace(raw);
    int n = get_number(raw, &rewind);
    for (int i = 0; i < n; i++) {
        input = rewind;
        while (input != NULL && input[0] != '\0') {
            if (input[0] == 'c' || input[0] == 'C') {
                total = 0;
                *state = CLR;
                input++;
            } else if (input[0] == '+') {
                if (f.verbose) 
                    printf("+ ");
                *state = ADD;
                input++;
            } else if (input[0] == '-') {
                if (f.verbose)
                    printf("- ");
                *state = SUB;
                input++;
            } else if (input[0] == '*') {
                if (f.verbose)
                    printf("* ");
                *state = MUL;
                input++;
            } else if (input[0] == '/') {
                if (f.verbose)
                    printf("/ ");
                *state = DIV;
                input++;
            } else if (input[0] == '=') {
                if (f.verbose)
                    printf("==============\n");
                printf("%d\n",total);
                if (f.verbose)
                    printf("\n");
                *state = CLR;
                input++;
            } else if (input[0] == ' ' || input[0] == '\t') {
                // ignore whitespace
            } else {
                char *left;
                diceset d = makeSet(input, &left);
                input = left;
                //printf("DEBUG: %s\n", left);
                if (d.sides >= 0 && d.num > 0) {
                    error = false;
                    int result = rollset(d, f);
                    if (*state == ADD) {
                        total += result;
                    } else if (*state == SUB) {
                        total -= result;
                    } else if (*state == MUL) {
                        total *= result;
                    } else if (*state == DIV) {
                        total /= result;
                    } else if (*state == CLR) {
                        total = result;
                    }
                    *state = CLR;
                } else {
                    if (!error)
                        printf("\n{ERROR near '%s'}\n",input);
                    input++;
                    error = true;
                }
            } // else if chain
        } // while
    } // for
    return total;
}
