#include <stdio.h>

int strend(char *s, char *t);

int main(void){
	char fullname[] = "Piers Morgan";
	char lastname[] = "Morgan";
	char notLastname[] = "Pears";
	int n;

	n = strend(fullname, lastname);
	if (n){
		printf("The lastname is correct\n");
	}

	n = strend(fullname, notLastname);
	if (!n){
		printf("The lastname is incorrect\n");
	}

	return 0;
}

int strend(char *s, char *t){
	char *bs = s;
	char *bt = t;

	while (*s) s++;
	while (*t) t++;
	
	while (bt < t){
		if (*--s != *--t){
			return 0;
		}
	}

	return 1;
}
