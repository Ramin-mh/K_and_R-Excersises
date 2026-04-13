#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAXOP 100 /* max size of operand/operator */
#define NUMBER '0'

/* === STACK === */
#define STACK_SIZE 100
double stack[STACK_SIZE];
int top = -1;

/* Input buffer for getch/ungetch */
char buf;
int bufp = 0;

void push(double val);
double pop(void);
char getop(char s[]); /* Input parsing */

int main(void){
    int type;
    char s[MAXOP];
    int s_top = -1;
    double op2;
    double tmp;
	double var[26] = {0.0};
	bool is_var_assign = false;

    /* input processing */
    while ((type = getop(s)) != EOF){
		if(is_var_assign){
			var[type - 'a'] = stack[top];
			is_var_assign = false;
			continue;
		}
		else if (type >= 'a' && type <= 'z'){
			push(var[type - 'a']);
			continue;
		}

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
            case '%':
                op2 = pop();
                if ((int)op2 == 0){
                    printf("error: zero divisor\n");
                    return EXIT_FAILURE;
                }
                push((int)pop() % (int)op2);
                break;
			case '=':
				is_var_assign = true;
				break;
            case '?': /* To print top element */
                if (top >= 0){
                    printf("top element: %.8g;\n", stack[top]);
                }
                break;
            case 'D': /* To duplicate top element */
                if (top >= 0){
                    top++;
                    stack[top] = stack[top - 1];
                }
                break;
            case '^': /* To swap top two elements */
                if (top > 0){
                    tmp = stack[top];
                    stack[top] = stack[top - 1];
                    stack[top - 1] = tmp;
                }
                break;
            case 'X': /* To clear the stack */
                top = -1;
                break;
            case 'S': /* sin function */
                push(sin(pop() * (M_PI/180)));
                break;
            case 'C': /* cos function */
                push(cos(pop() * (M_PI/180)));
                break;
            case 'T': /* tan function */
                push(tan(pop() * (M_PI/180)));
                break;
            case 'E': /* exponent function */
                push(exp(pop()));
                break;
            case 'P': /* pow function pow(x,y) = x^y */
                op2 = pop();
                push(pow(pop(), op2));
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
        printf("push: Stack overflow!, exiting...");
        exit(EXIT_FAILURE);
    }
}

double pop(void){
    if (top > -1){
        return stack[top--];
    }
    else {
        printf("pop: Stack underflow!, exiting...");
        exit(EXIT_FAILURE);
    }
}

char getch(void){
    /* if there is a value in buffer, return it. otherwise return getchar() */
	if (bufp){
		bufp--;
		return buf;
	}
	else {
		return getchar();
	}
}

void ungetch(char c){
    /* store given char into the buffer */
    if (bufp){
        printf("ungetch: Buffer overflow!, exiting...");
        exit(EXIT_FAILURE);
	}
	buf = c;
	bufp++;
}

void ungets(char s[]){
	/* store given string into the buffer */
	int len, i;
	for (len = 0; s[len] != '\0'; len++);

	for (i = len - 1; i >= 0; i--){
		ungetch(s[i]);
	}
}

/* Input parsing */
char getop(char s[]){
    int i = 0;
    char c, tmp;

    while (((s[0] = c = getch()) == ' ') || (c == '\t')) { } /* Skip whitespace */
    s[1] = '\0';

    /* Handle negetive numbers */
    if (c == '-'){
        if (isdigit(tmp = getch())){
            s[++i] = c = tmp;
        }
        else {
            ungetch(tmp);
        }
    }

    if ((!isdigit(c)) && (c != '.')){
        return c; /* Not a number */
    }

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
