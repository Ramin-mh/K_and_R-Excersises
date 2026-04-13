#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINES 100
#define MAXLEN 1000

char *alloc(int);
int readlines(char *s[]);
void writelines(char *s[], int line_len, int start);

int main(int argc, char *argv[]){
    char *s[LINES];
    int line_len;
    int i;
    int n;
    int start = 0;

    if (argc == 1){
        n = 10;
    }
    else if (argc == 2){
        // The parsing only allows non-negetive integers.
        if (*argv[1] != '-'){
            fprintf(stderr, "invalid argument. exiting...\n");
            return 1;
        }
        for (i = 1; isdigit(*(argv[1] + i)); i++);
        if (i == 1 || *(argv[1] + i) != '\0'){
            fprintf(stderr, "invalid argument. exiting...\n");
            return 1;
        }

        n = atoi((argv[1] + 1));
    }
    else {
        fprintf(stderr, "only one argument allowed. exiting...\n");
        return 1;
    }

    line_len = readlines(s);

    if (n < line_len){
        start = line_len - n;
    }
    
    writelines(s, line_len, start);

    return 0;    
}

char *alloc(int n) {
    static char buf[LINES * MAXLEN];
    static int next_free = 0;
    char *block_start = buf + next_free;
 
    if (n > 0 && n <= LINES * MAXLEN - next_free) {
        next_free += n;
        return block_start;
    } else {
        fprintf(stderr, "Buffer Overflow, Allocation Failed!\n");
        return NULL;
    }
}

int readlines(char *s[]){
    int c;
    int line_index = -1;
    char line[MAXLEN];
    int next_free;

    while (1){
        // Line input start---
        next_free = 0;
        while ((c = getchar()) != EOF && c != '\n'){
            if (next_free < MAXLEN - 2){
                line[next_free++] = c;
            }
            else {
                while ((c = getchar()) != EOF && c != '\n') { }
                break;
            }
        }

        // don't store line if nothing entered in that line
        if (c == EOF && next_free == 0) break;

        if (c == '\n'){
             line[next_free++] = '\n';
        }
        line[next_free] = '\0';
        // Line input end...

        // Line saving start---
        if (line_index < LINES - 1){
            if ((s[++line_index] = alloc(next_free + 1)) == NULL){
                fprintf(stderr, "Memory allocation failed, closing the function\n");
                break;
            }
            strcpy(s[line_index], line);
        }
        else {
            fprintf(stderr, "maximum allocated lines exceeded, closing the function\n");
            break;
        }
        // Line saving end...

        if (c == EOF) break;
    }

    /* Return the total no.of lines */
    return line_index + 1;
}

void writelines(char *s[], int line_len, int start){
    int i;

    for (i = start; i < line_len; i++){
        printf("%s", *(s + i));
    }
}
