#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int (*convert_text)(int);
    int c;

    if (argc == 2)
    {
        if (*(argv[1]) == 'u')
        {
            convert_text = toupper;   
        }
        else if (*(argv[1]) == 'l')
        {
            convert_text = tolower;
        }
        else
        {
            fprintf(stderr, "invalid argument, exiting...\n");
            return 1;
        }
    }
    else {
        fprintf(stderr, "argument required, exiting\n");
        return 1;
    }

    while ((c = getchar()) != EOF)
    {
        putchar(convert_text(c));
    }

    return 0;
}
