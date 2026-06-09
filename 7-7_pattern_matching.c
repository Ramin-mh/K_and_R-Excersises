#include <stdio.h>
#define MAXLINE 1000

int strindex(char *source, char *searchfor);

int main(int argc, char *argv[])
{
    char *pattern;

    char *line;
    size_t size;

    FILE *fp;
    char f_line[MAXLINE];

    if (argc < 2)
    {
        fprintf(stderr, "usage: <program> <pattern> [<filename> ...]\n");
        return 1;
    }

    pattern = *++argv;

    if (argc == 2)
    {
        while (getline(&line, &size, stdin) != -1)
        {
            if (strindex(line, pattern) != -1)
            {
                printf("%s", line);
            }
        }
    }
    else
    {
        while (--argc > 1)
        {
            if ((fp = fopen(*++argv, "r")) == NULL)
            {
                fprintf(stderr, "file: %s failed to open\n", *argv);
                return 1;
            }

            printf("\nfilename: %s\n", *argv);
            while (fgets(f_line, MAXLINE, fp) != NULL)
            {
                if (strindex(f_line, pattern) != -1)
                {
                    printf("%s", f_line);
                }
            }

            fclose(fp);
        }
    }

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
