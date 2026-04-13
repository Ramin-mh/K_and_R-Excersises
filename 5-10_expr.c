#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER '0'
#define INVALID '1'
#define MAXOP 100

char getop(char *arg, char *s);
char parse_number(char *arg, char *s);
void push(double);
double pop(void);

double stack[MAXOP];
int top = -1;

int main(int argc, char *argv[]){
    int i;
    double op2;
    char number[100];

    for (i = 1; i < argc; i++){
        switch (getop(*(argv + i), number)){
            case NUMBER:
                push(atof(number));
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
                if (!op2){
                    fprintf(stderr, "error: division by zero\n");
                    exit(1);
                }
                push(pop() / op2);
                break;
            default:
                fprintf(stderr, "Invalid Input!\n");
        }       
    }

    if (top == 0){
        printf("ans: %g\n", stack[top]);
    }
    else {
        printf("Not enough operators to get final result!\n");
        printf("Anyway here is the result of last done operation: %g\n", stack[top]);
    }

    return 0;
}

void push(double n){
    if (top < MAXOP-1){
        stack[++top] = n;
    }
    else {
        fprintf(stderr, "Stack Overflow!\n");
        exit(1);
    }
}

double pop(void){
    if (top > -1){
        return stack[top--];
    }
    else {
        fprintf(stderr, "Stack Underflow!");
        exit(1);
    }
}

char getop(char *arg, char *s){
    char c;

    // Handle positive number
    if (isdigit(*arg) || *arg == '.'){
        return parse_number(arg, s);
    }

    // Handle negetive number
    if ((*arg == '-') && (isdigit(*(arg + 1)) || *(arg + 1) == '.')){
        return parse_number(arg, s);
    }

    return *arg;
}

char parse_number(char *arg, char *s){
    *s = *arg;
    while (isdigit(*++s = *++arg)) { } // get integer part (including negetive number)

    if (*s == '.'){
        while (isdigit(*++s = *++arg)) { } // get decimal part
    }

    if (*s != '\0'){
        return INVALID;
    }
    else {
        return NUMBER;
    }
}
