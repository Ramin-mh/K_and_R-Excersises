#include <stdio.h>
#define SIZE 100

void strncpy_using_pointers(char *s, const char *t, int n);

int main(void){
	char s1[SIZE] = "<12> man";
	char s2[SIZE] = "This is a string";

	printf("Before strncpy: %s\n", s1);
	strncpy_using_pointers(s1, s2, 4);
	printf("After strncpy: %s\n", s1);

	return 0;
}

void strncpy_using_pointers(char *s, const char *t, int n){
	while (n && (*s++ = *t++)) n--;
}
