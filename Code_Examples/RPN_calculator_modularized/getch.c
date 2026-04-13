#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#define BUFSIZE 100

static char buf[BUFSIZE];
static int bufp = 0;

int getch(void){
    /* if there is a value in buffer, return it. otherwise return getchar() */
    return (bufp > 0)? buf[--bufp] : getchar();
}

void ungetch(int c){
    /* store given char into the buffer */
    if (bufp < BUFSIZE){
        buf[bufp++] = c;
    }
    else {
        printf("ungetch: buffer is full!, exiting...");
        exit(EXIT_FAILURE);
    }
}
