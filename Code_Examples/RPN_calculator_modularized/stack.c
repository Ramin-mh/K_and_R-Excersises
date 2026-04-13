#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#define STACK_SIZE 100

static double stack[STACK_SIZE];
static int top = -1;

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
