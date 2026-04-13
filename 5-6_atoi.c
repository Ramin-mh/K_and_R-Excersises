#include <stdio.h>
#include <ctype.h>

int atoi_using_pointers(char *);

int main(void){
	char nums[] = "567kl";
	int n = atoi_using_pointers(nums);

	printf("%d\n", n);
	return 0;
}

int atoi_using_pointers(char *s){
	int n = 0, sign = 1;
	while (*s == ' ') s++; /* Skip whitespaces */

	if (*s == '-'){
		sign = -1;
		s++;
	}

	while (isdigit(*s)){
		n = (10 * n) + (*s - '0');
		s++;
	}

	return n * sign;
}
