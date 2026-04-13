#include <stdio.h>

void strncat_using_pointers(char *s, int n, char *t);

int main(void){
	char str1[100] = "i am ";
	char str2[100] = "a man that does what?";
	strncat_using_pointers(str2, 5, str1);
	printf("%s\n", str1);

	return 0;
}

void strncat_using_pointers(char *s, int n, char *t){
	while (*t) t++;
	while (n && (*t++ = *s++)) n--;
}
