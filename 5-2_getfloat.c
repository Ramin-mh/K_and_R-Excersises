#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int getfloat(float *);
int getch(void);
void ungetch(int);

int main(void){
	float n;
	int c;

	c = getfloat(&n);
	printf("The number is: %g\n", n);

	return EXIT_SUCCESS;
}

int getfloat(float *pn){
	int c, sign, multiplier = 0;

	while (isspace(c = getch())) /* Skip whitespace */
		;
	
	if (!isdigit(c) && c != EOF &&c != '+' && c != '-' && c != '.'){
		ungetch(c); /* It is not a number */
		return 0;
	}

	sign = (c == '-')? -1 : 1;
	
	if (c == '+' || c == '-'){
		if (!isdigit(c = getch())){
			ungetch(c); /* It is not a number */
			return 0;
		}
	}

	/* Get integer part */
	for (*pn = 0; isdigit(c); c = getch()){
		*pn = *pn * 10 + (c - '0');
	}
	*pn *= sign;

	if (c == '.'){
		while (isdigit(c = getch())){
			*pn = *pn * 10 + (c - '0');
			multiplier++;
		}

		while (multiplier--){
			*pn /= 10;
		}
	}

	if (c != EOF){
		ungetch(c);
	}

	return c;
}

#define SIZE 1
int stack[SIZE];
int top = -1;

int getch(void){
	return (top > -1)? stack[top--] : getchar();
}

void ungetch(int c){
	if (top < (SIZE - 1)){
		stack[++top] = c;
	}
	else {
		fprintf(stderr, "Stack overflow!");
		exit(EXIT_FAILURE);
	}
}
