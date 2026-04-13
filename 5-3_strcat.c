#include <stdio.h>
#define SIZE 100

void strcat_using_pointers(char *s, char *t);

int main(void){
	char s1[SIZE] = "the daily ne";
	char s2[SIZE] = "ws of today is here.";

	strcat_using_pointers(s1, s2);
	printf("%s\n", s1);

	return 0;
}

void strcat_using_pointers(char *s, char *t){
	for (; *s; s++)
		;

	while ((*s++ = *t++))
		;
}
