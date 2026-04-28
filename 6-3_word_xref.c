#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

int getword(char *w, int size, int *line_no);

int main(void) {
    char w[MAXWORD];
    int k;
    int line_no = 1;

    while ((k = getword(w, MAXWORD, &line_no)) != EOF && k != '\0') {
        printf("%d: %s\n", line_no, w);
    }

    if (k == '\0') {
        fprintf(stderr, "the word exceeded given size.\n");
        return 1;
    }

    return 0;
}

int is_noise_word(char *s) {
    return strcmp(s, "and") == 0 ||
           strcmp(s, "the") == 0 ||
           strcmp(s, "or") == 0 ||
           strcmp(s, "a") == 0 ||
           strcmp(s, "when") == 0;
}

void to_lower(char *s) {
    for (; *s; s++) {
        *s = tolower(*s);
    }
}

static int buf = EOF;

int getch(void) {
    int tmp;
    
    if (buf == EOF) {
        return getchar();
    }
    else {
        tmp = buf;
        buf = EOF;
        return tmp;    
    }
}

void ungetch(int c) {
    if (buf == EOF) {
        buf = c;
    }
    else {
        fprintf(stderr, "buffer overflow!\n");
        exit(1);
    }
}

int getword(char *w, int size, int *line_no) {
    char *p;
    int i;
    int c;

    while (1) {
        p = w;
        i = size;
        
        /* Skip non-alphabets */
        while (!isalpha((c = getch()))) {
            if (c == EOF) return EOF;
            if (c == '\n') (*line_no)++;
        }

        do {
            *p++ = c;
            i--;
            if (i < 1) {
                *(--p) = '\0';
                return '\0';
            }
        } while (isalpha((c = getch())));
        *p = '\0';

        ungetch(c); /* leave the char to be checked by the top loop */
        
        to_lower(w);
        if (!is_noise_word(w)) {
            return w[0];
        }
    }
}
