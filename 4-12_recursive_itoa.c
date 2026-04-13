#include <stdio.h>
#include <stdlib.h>

char* recursive_itoa_helper(int n, char s[]);
void recursive_itoa(int n, char s[]);

int main(void){
	int n = -145;
	char s[100];

	recursive_itoa(n, s);
	printf("%s\n", s);

	return EXIT_SUCCESS;
}

char* recursive_itoa_helper(int n, char s[]){
	if (n < 0){
		*s++ = '-';
		n = -n;
	}

	if (n / 10){
		s = recursive_itoa_helper(n/10, s);
	}
	*s++ = (n % 10) + '0';
	return s;
}

void recursive_itoa(int n, char s[]){
	char * end = recursive_itoa_helper(n, s);
	*end = '\0';
}

