#include <stdio.h>

#define MAXLINE 1000

char *fgets_line(char *s, int n, FILE *iop);
char string_cmp(char *s1, char *s2);

int main(int argc, char *argv[])
{
    FILE *f1, *f2;
    char *stat1, *stat2;
    char s1[MAXLINE], s2[MAXLINE];

    if (argc != 3)
    {
        fprintf(stderr, "usage: program filename filename\n");
        return 1;
    }

    f1 = fopen(argv[1], "r");
    f2 = fopen(argv[2], "r");

    while (1)
    {
        stat1 = fgets_line(s1, MAXLINE, f1);
        stat2 = fgets_line(s2, MAXLINE, f2);

        if (stat1 == NULL && stat2 == NULL)
            break;

        if (string_cmp(s1, s2) != 0){
            printf("%s: %s\n", argv[1], s1);
            printf("%s: %s\n", argv[2], s2);
            break;
        }
    }

    fclose(f1);
    fclose(f2);

    return 0;
}

char *fgets_line(char *s, int n, FILE *iop)
{
    register int c;
    register char *cs;

    cs = s;
    while (--n > 0 && (c = getc(iop)) != EOF)
    {
        if ((*cs++ = c) == '\n')
            break;
    }
    *cs = '\0';
    
    return (c == EOF && cs == s)? NULL : s;
}

char string_cmp(char *s1, char *s2)
{
    for (; *s1 == *s2; s1++, s2++)
    {
        if (*s1 == '\0')
           break;
    }
    return *s1 - *s2;
}
