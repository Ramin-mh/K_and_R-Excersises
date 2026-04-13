#include <stdio.h>

void itoa_using_pointers(int, char *);

int main(void){
	int n = -78349;
	char nums[100];
	itoa_using_pointers(n, nums);

	printf("%s\n", nums);

	return 0;
}

void itoa_using_pointers(int n, char *s){
	int t;
	char *k;
	char tmp;

	if (!n){
		*s = '0';
		return;
	}

	if (n < 0){
		*s++ = '-';
		n = -n;
	}

	k = s;

	while (n){
		t = n % 10;
		*s++ = t + '0';
		n /= 10;
	}
	*s-- = '\0';

	for (; k < s; k++, s--){
		tmp = *k;
		*k = *s;
		*s = tmp;
	}
}
