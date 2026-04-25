#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

struct key {
    char *word;
    int count;
};

struct key keytab[] = {
    {"auto", 0},
    {"break", 0},
    {"char", 0},
    {"double", 0},
    {"else", 0},
    {"float", 0},
    {"for", 0},
    {"goto", 0},
    {"if", 0},
    {"int", 0},
    {"void", 0},
    {"while", 0}
};

#define NKEYS ((sizeof keytab) / (sizeof keytab[0]))

int getword(char *word, int lim);
int binsearch(char *word, struct key tab[], int n);

int main(void) {
    char word[MAXWORD];
    int n;

    while (getword(word, NKEYS) != EOF){
        if (isalpha(word[0])) {
            if ((n = binsearch(word, keytab, NKEYS)) >= 0) {
                keytab[n].count++;
            }
        }        
    }

    printf("\n----------------\n");
    for (n = 0; n < NKEYS; n++) {
        if (keytab[n].count > 0) {
            printf("%4d %s\n", keytab[n].count, keytab[n].word);
        }
    }

    return 0;
}

static int buf = EOF;

int getch(void) {
    int tmp;

    if (buf != EOF) {
        tmp = buf;
        buf = EOF;
        return tmp;
    }
    return getchar();
}

void ungetch(int c) {
    if (buf != EOF){
        fprintf(stderr, "Buffer Overflow!\n");
        exit(1);
    }
    buf = c;
}

int getword(char *word, int lim) {
    int c, tmp;
    char *w = word;

    while ((c = getch()) != EOF) {
        if (isspace(c)) continue;

        /* Skip String and Char constants */
        if (c == '"' || c == '\'') {
            tmp = c;
            while ((c = getch()) != tmp && c != EOF) {
                /* Skip the \ in string or char to prevent exiting early */
                if (c == '\\') {
                    getch();
                }
            }
            continue;
        }

        /* Skip Preprocessor Control Lines */
        if (c == '#') {
            while ((c = getch()) != '\n' && c != EOF) { }
            if (c == EOF) {
                *w = '\0';
                return EOF;
            }
            continue;
        }

        /* Skip Comments */
        if (c == '/') {
            if ((tmp = getch()) == '*') {
                while (c != EOF) {
                    c = getch();
                    if (c == '*'){
                        c = getch();
                        if (c == '/') {
                            break;
                        }
                        else if (c == '*'){
                            ungetch(c);
                        }
                    }
                }
                continue;
            }
            else {
                ungetch(tmp);
            }
        }
    
        if (c != EOF && c != '\n'){
            *w++ = c;
        }
        if (!isalpha(c)){
            *w = '\0';
            return c;
        }

        for (; --lim > 0; w++){
            if (!isalnum((*w = getch())) && *w != '_') {
                ungetch(*w);
                break;
            }
        }
        *w = '\0';
        return word[0];
    }
    *w = '\0';
    return EOF;
}

int binsearch(char *word, struct key tab[], int n) {
    int cond;
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high){
        mid = (low + high) / 2;

        if ((cond = strcmp(word, tab[mid].word)) > 0) {
            low = mid + 1;
        }
        else if (cond < 0) {
            high = mid - 1;
        }
        else {
            return mid;
        }
    }
    return -1;
}
