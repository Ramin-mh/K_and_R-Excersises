#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100 /* max size of operand/operator */
#define NUMBER '0'

/* === STACK === */
#define STACK_SIZE 100
double stack[STACK_SIZE];
int top = -1;

/* Input buffer for getch/ungetch */
#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;

void push(double val);
double pop(void);
char getop(char s[]); /* Input parsing */

int main(){
    int type;
    char s[MAXOP];
    double op2;

    /* input processing */
    while ((type = getop(s)) != EOF){
        switch (type){
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '*':
                push(pop() * pop());
                break;
            case '/':
                op2 = pop();
                if (op2 == 0.0){
                    printf("error: zero divisor\n");
                    return EXIT_FAILURE;
                }
                push(pop() / op2);
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }

    return EXIT_SUCCESS;
}

void push(double val){
    if (top < (STACK_SIZE - 1)){
        stack[++top] = val;
    }
    else {
        printf("Stack overflow!, exiting...");
        exit(EXIT_FAILURE);
    }
}

double pop(void){
    if (top > -1){
        return stack[top--];
    }
    else {
        printf("Stack underflow!, exiting...");
        exit(EXIT_FAILURE);
    }
}

char getch(void){
    /* if there is a value in buffer, return it. otherwise return getchar() */
    return (bufp > 0)? buf[--bufp] : getchar();
}

void ungetch(char c){
    /* store given char into the buffer */
    if (bufp < BUFSIZE){
        buf[bufp++] = c;
    }
    else {
        printf("ungetch: buffer is full!, exiting...");
        exit(EXIT_FAILURE);
    }
}

/* Input parsing */
char getop(char s[]){
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
