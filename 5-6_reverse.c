#include <stdio.h>

void reverse(char *);

int main(void){
	char name[] = "srotcurtsnoc";
	reverse(name);
	printf("%s\n", name);
	return 0;
}

void reverse(char *s){
	char tmp;
	int i;
	char *a, *b;

	a = s;
	for (i = 0; *(s+i) != '\0'; i++); // set i to index where \0 is
	b = s + i - 1;

	for (; a < b; a++, b--){
		tmp = *a;
		*a = *b;
		*b = tmp;
	}
}
