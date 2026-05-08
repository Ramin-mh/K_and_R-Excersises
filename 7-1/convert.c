#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int (*convert_text)(int);
    int c;

    if (strcmp(argv[0], "./bin/upper") == 0)
    {
        convert_text = toupper;
    }
    else if (strcmp(argv[0], "./bin/lower") == 0)
    {
        convert_text = tolower;
    }
    else
    {
        fprintf(stderr, "error, exiting..\n");
        return 1;
    }
    
    while ((c = getchar()) != EOF)
    {
        putchar(convert_text(c));
    }

    return 0;
}
