#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#define MAXOP 100

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
