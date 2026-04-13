#include <stdio.h>
#include <stdlib.h>

void recursive_reverse(char s[], int len);
void reverse(char s[], int len);

int main(void){
	char s[] = "dirdam laer";
	int len = sizeof(s) - 1;

	recursive_reverse(s, len);
	printf("%s\n", s);

	return EXIT_SUCCESS;
}

void recursive_reverse_helper(char s[], int i, int last){
	int tmp;

	if (i >= last){
		return;
	}

	recursive_reverse_helper(s, i+1, last-1);

	tmp = s[i];
	s[i] = s[last];
	s[last] = tmp;
}

void recursive_reverse(char s[], int len){
	recursive_reverse_helper(s, 0, len-1);
}

void reverse(char s[], int len){
	int i, tmp;
	int last = len - 1;
	for (i = 0; i < last; i++, last--){
		tmp = s[i];
		s[i] = s[last];
		s[last] = tmp;
	}
}
