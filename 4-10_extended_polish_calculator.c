#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXOP 100 /* max size of operand/operator */
#define NUMBER '0'

/* === STACK === */
#define STACK_SIZE 100
double stack[STACK_SIZE];
int top = -1;

/* Input buffer for getch/ungetch */
int buf;
int bufp = 0;

void push(double val);
double pop(void);
int getAline(char s[], int lim);
void process_input(void);

int main(void){
	while (true){
		process_input();
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

int getAline(char s[], int lim){
    int c, i;

    for (i = 0; i < lim - 1; i++) {
        c = getchar();
        if (c == EOF || c == '\n')
            break;
        s[i] = c;
    }

    s[i] = '\n';      // keep newline if you want
    s[i + 1] = '\0';  // VERY IMPORTANT
    return i + 1;
}

void process_input(void){
	char s[MAXOP];
	int s_bottom = 0, s_top;
	double number = 0, divide_factor = 0;
	double op2;
	bool is_negetive = false;

	s_top = getAline(s, MAXOP);

	while (s_bottom < (s_top - 1)){
		if (s[s_bottom] == ' '){
			if (isdigit(s[s_bottom  - 1])){
				/* Push the number */

				if (is_negetive){
					number = 0 - number;
				}

				for (; divide_factor > 0; divide_factor--){
					number /= 10;
				}

				push(number);

				number = 0;
				is_negetive = false;
				divide_factor = 0;
			}
			s_bottom++;
		}

		if (s[s_bottom] == '-'){
			if (isdigit(s[s_bottom + 1])){
				is_negetive = true;
				s_bottom++;
			}
		}

		if (!isdigit(s[s_bottom]) && s[s_bottom] != '.'){
			/* Handle operation */
			switch (s[s_bottom]){
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
						exit(EXIT_FAILURE);
					}
					push(pop() / op2);
					break;
				case '%':
					op2 = pop();
					if ((int)op2 == 0){
						printf("error: zero divisor\n");
						exit(EXIT_FAILURE);
					}
					push((int)pop() % (int)op2);
					break;
				case '\n':
					break;
				default:
					printf("error: unknown command %c\n", s[s_bottom]);
					break;
			}

			s_bottom++;
		}

		while (isdigit(s[s_bottom])){
			number = number * 10 + (s[s_bottom] - '0');
			s_bottom++;
		}

		if (s[s_bottom] == '.'){
			s_bottom++;
			while (isdigit(s[s_bottom])){
				number = number * 10 + (s[s_bottom] - '0');
				s_bottom++;
				divide_factor++;
			}
		}
	}
	
	printf("\t\t%.8g\n", pop());
	top = -1;
}
