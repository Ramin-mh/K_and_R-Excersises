#include <stdio.h>
#include <ctype.h>
#include "calc.h"

int getop(char s[]){
    int i;
    char c;
	static int lastc = 0;

	if (lastc != 0){
		c = lastc;
		lastc = 0;
	}
	else {
		c = getchar();
	}

	/* Skip whitespace */
    while (c == ' ' || c == '\t'){
		c = getchar();
	}

    if (!isdigit(c) && c != '.'){
        return c; /* Not a number */
    }

    i = 0;
	/* collect integer part */
    if (isdigit(c)){
        while (isdigit(c)){
			s[i++] = c;
			c = getchar();
		}
	}

	/* get decimal part */
    if (c == '.'){
		s[i++] = '.';
		c = getchar();
        while (isdigit(c)){
			s[i++] = c;
			c = getchar();
		}
	}

    s[i] = '\0';

    if (c != EOF){
        lastc = c;
    }

  return NUMBER;
}
