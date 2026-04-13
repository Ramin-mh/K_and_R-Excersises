#include <stdio.h>

int strncmp_using_pointers(char *s1, char *s2, int n);

int main(void){
	char name[100] = "Jacob";
	char new_name[100] = "Jakeb";
	int n = 6, k;

	k = strncmp_using_pointers(name, new_name, n);

	if (!k){
		printf("both are same\n");
	}
	else {
		printf("Not same, off by: %d\n", k);
	}

	return 0;
}

int strncmp_using_pointers(char *s1, char *s2, int n){
	for (; *s1 == *s2; s1++, s2++, n--){
		if (!n || *s1 == '\0'){
			return 0;
		}
	}

	return *s1 - *s2;
}
