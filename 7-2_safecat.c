#include <stdio.h>
#include <ctype.h>

#define MAXLINE 80

int main(void)
{
    int c;
    int len;

    len = 0;
    while ((c = getchar()) != EOF) {
        if (len >= MAXLINE)
        {
            putchar('\n');
            len = 0;
        }
        
        if (isprint(c))
        {
            putchar(c);
            len++;
        }
        else
        {
            printf("\\x%02X", c);
            len += 4;
        }
    }
}
