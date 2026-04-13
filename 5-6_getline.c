#include <stdio.h>

int getline_using_pointers(char *s, int n);

int main(void){
	return 0;
}

int getline_using_pointers(char *s, int n){
	int c;
	char *start = s;

	while (--n > 0 && (c = getchar()) != '\n' && c != EOF){
		*s++ = c;
	}
	*s = '\0';

	return s - start;
}
