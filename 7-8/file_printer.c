#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 5
#define MAXLINE 1000

int main(int argc, char *argv[])
{
    FILE *fp;
    char line[MAXLINE];
    int i, page;

    if (argc == 1)
    {
        fprintf(stderr, "No files given to print!, exiting...\n");
        exit(1);
    }

    while (--argc > 0)
    {
        if ((fp = fopen(*++argv, "r")) == NULL)
        {
            fprintf(stderr, "The file %s failed to open, abandoning this file and continuing...\n", *argv);
            continue;
        }

        page = 1;
        for (i = 0; fgets(line, MAXLINE, fp) != NULL;)
        {
            if (i == 0)
            {
                fputs("-------------------------\n", stdout);
                if (page == 1)
                {
                    fprintf(stdout, "contents of %s\n", *argv);
                    i++;
                }
                fprintf(stdout, "page: %d\n", page);
                i++;
            }

            fputs(line, stdout);
            i++;

            if (i >= 5)
            {
                i = 0;
                page++;
            }
        }
        if (i > 0)
            while (i++ < 5) fputs("\n", stdout);

        fclose(fp);
    }
    fputs("-------------------------\n", stdout);

    return 0;
}
