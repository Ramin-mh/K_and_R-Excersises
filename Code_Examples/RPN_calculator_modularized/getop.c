#include <stdio.h>
#include <ctype.h>
#include "calc.h"

int getop(char s[]){
    int i;
    char c;

    while ((s[0] = c = getch()) == ' ' || c == '\t') { } /* Skip whitespace */
    s[1] = '\0';

    if (!isdigit(c) && c != '.'){
        return c; /* Not a number */
    }

    i = 0;
    if (isdigit(c)){
        while (isdigit(s[++i] = c = getch())) { } /* collect integer part */
    }

    if (c == '.'){
        while (isdigit(s[++i] = c = getch())) { } /* get decimal part */
    }

    s[i] = '\0';

    if (c != EOF){
        ungetch(c);
    }

    return NUMBER;
}
