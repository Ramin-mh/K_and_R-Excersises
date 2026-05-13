#include <stdio.h>

#define MAXBUF 1000

int main(void)
{
    double stack[MAXBUF];
    int top = -1;
    double op1, op2;
    
    char s[MAXBUF];

    double n;
    char c;

    /* works only for space seperated input */
    while (scanf("%s", s) == 1)
    {
        if (sscanf(s, "%lf", &n) == 1)
        {
            stack[++top] = n;
        }
        else if (sscanf(s, "%c", &c) == 1)
        {
            switch (c)
            {
                case '+':
                    op2 = stack[top--];
                    stack[top] += op2;
                    break;
                case '-':
                    op2 = stack[top--];
                    stack[top] -= op2;
                    break;
                case '*':
                    op2 = stack[top--];
                    stack[top] *= op2;
                    break;
                case '/':
                    op2 = stack[top--];
                    stack[top] /= op2;
                    break;
            }
        }
    }

    printf("%lf\n", stack[top]);
    
    return 0;   
}
