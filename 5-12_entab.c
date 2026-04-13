#include <stdio.h>
#include <stdlib.h>

#define TABSTOP 4
#define DEFAULT_INCREMENTOR 1
#define MAX_TABSTOPS 250

void entab(int tabstops[], int size, int incrementor);
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

    entab(tabstops, size, incrementor);

    return 0;
}

int buffer = EOF;

int getch(void){
    int tmp;
    
    if (buffer == EOF) {
        return getchar();
    }

    tmp = buffer;
    buffer = EOF;
    return tmp;
}

void ungetch(int c){
    if (buffer != EOF){
        fprintf(stderr, "Buffer overflow!, exiting...\n");
        exit(1);
    }
    buffer = c;    
}

void entab(int tabstops[], int size, int incrementor){
    int c, col, spaces, tabstop, tabstops_i, i;
    
    while (1) {
        col = 0;
        tabstop = tabstops[0];
        tabstops_i = 0;
        
        while ((c = getch()) != EOF && c != '\n'){
            if (c == ' '){
                while (col >= tabstop){
                    if (tabstops_i < size - 1){
                        tabstop = tabstops[++tabstops_i];
                    }
                    else {
                        tabstop += incrementor;
                    }
                }
                spaces = tabstop - col;
                for (i = 1; i < spaces && (c = getch()) == ' '; i++);
                col += i;
                if (i == spaces) {
                    printf("/t");
                }
                else {
                    if (c == EOF) break;
                    ungetch(c);
                    while (i > 0) {
                        putchar(' ');
                        i--;
                    }
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
