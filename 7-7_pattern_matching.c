#include <stdio.h>

#define MAXLINE 1000

int strindex(char *source, char *searchfor);

int main(int argc, char *argv[])
{
    char *line = "abc";
    char *pattern = "ab";

    printf("%d\n", strindex(line, pattern));

    return 0;
}

int strindex(char *s, char *t)
{
    int i, j, k;

    for (i = 0; s[i]; s++)
    {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++);

        if (k > 0 && t[k] == '\0')
            return i;
    }

    return -1;
}
