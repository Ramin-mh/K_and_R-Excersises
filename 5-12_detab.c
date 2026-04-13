#include <stdio.h>
#include <stdlib.h>

#define TABSTOP 4
#define DEFAULT_INCREMENTOR 1
#define MAX_TABSTOPS 250

void detab(int tabstops[], int tabstops_size, int start);
int is_shorthand(int argc, char *argv[]);
int parse_shorthand(char *arg1, char *arg2, int *tabstop, int *incrementor);
int parse_tabstoplist(int argc, char *argv[], int *tabstops);

int main(int argc, char *argv[]) {
    int tabstops[MAX_TABSTOPS];
    int size = 1;
    int incrementor = DEFAULT_INCREMENTOR;

    if (argc - 1 > MAX_TABSTOPS) {
        fprintf(stderr, "exceeded maximum argument size (250 tabstops limit)\n");
        return 1;
    }

    if (argc == 1) {
        /* no args: use default tabstop */
        tabstops[0] = TABSTOP;
    }
    else if (is_shorthand(argc, argv)){
        /* shorthand given, parse and save */
        if (parse_shorthand(argv[1], argv[2], tabstops, &incrementor) == 0){
            fprintf(stderr, "invalid shorthand, exiting...\n");
            return 1;
        }
    }
    else {
        /* validate and parse for tabstop list given as args */
        if (parse_tabstoplist(argc, argv, tabstops) == 0) {
            fprintf(stderr, "invalid arguments, exiting...\n");
        }
        size = argc - 1;
    }

    detab(tabstops, size, incrementor);
    return 0;
}

void detab(int *tabstops, int tabstops_size, int incrementor) {
    int c;
    int col;
    int tabstop_i;
    int tabstop;
    int spaces, i;

    while (1){
        col = 0;
        tabstop_i = 0;
        tabstop = tabstops[0];

        while ((c = getchar()) != EOF && c != '\n') {
            if (c == '\t') {
                /* advance to next tabstop if we've passed the current one */
                while (col >= tabstop){
                    if (tabstop_i < tabstops_size - 1) {
                        tabstop = tabstops[++tabstop_i];
                    }
                    else {
                        tabstop += incrementor;
                    }
                }
                /* expand tab to spaces up to the tabstop boundary */
                spaces = tabstop - col;
                for (i = 0; i < spaces; i++) {
                    putchar(' ');
                    col++;
                }
            }
            else {
                putchar(c);
                col++;
            }
        }
        if (c == EOF) break;
        putchar('\n');
    }
}

int is_shorthand(int argc, char *argv[]){
    int i;

    if (argc != 3) return 0;

    if (*argv[1] != '-') return 0;

    if (*argv[2] != '+') return 0;

    return 1;
}

int parse_shorthand(char *arg1, char *arg2, int *tabstop, int *incrementor) {
    int x, y;

    if ((x = atoi(arg1+1)) <= 0) return 0;

    if ((y = atoi(arg2+1)) <= 0) return 0;

    *tabstop = x;
    *incrementor = y;

    return 1;
}

int parse_tabstoplist(int argc, char *argv[], int *tabstops){
    int i, n;
    int prev = 0;

    for (i = 1; i < argc; i++) {
        n = atoi(argv[i]);
        if (n <= prev) return 0;
        tabstops[i-1] = n;
        prev = n;
    }

    return 1;
}
