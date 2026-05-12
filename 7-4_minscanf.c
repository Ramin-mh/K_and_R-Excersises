#include <stdio.h>
#include <stdarg.h>

void minscanf(char *fmt, ...);

int main(void)
{
    char s[100];

    minscanf("%s", s);
    printf("%s\n", s);
    return 0;
}

void minscanf(char *fmt, ...)
{
    va_list ap;
    char *pc;
    int *pn;
    float *pf;
    unsigned int * pui;

    char *p;

    va_start(ap, fmt);
    for (p = fmt; *p; p++)
    {
        if (*p != '%')
            continue;

        switch(*++p)
        {
            case 'i':
                pn = va_arg(ap, int *);
                scanf("%i", pn);
            case 'd':
                pn = va_arg(ap, int *);
                scanf("%d", pn);
                break;
            case 'f':
                pf = va_arg(ap, float *);
                scanf("%f", pf);
                break;
            case 'u':
                pui = va_arg(ap, unsigned int *);
                scanf("%u", pui);
                break;
            case 'x':
                pn = va_arg(ap, int *);
                scanf("%x", pn);
                break;
            case 'c':
                pc = va_arg(ap, char *);
                scanf("%c", pc);
                break;
            case 's':
                pc = va_arg(ap, char *);
                scanf("%s", pc);
                break;
        }
    }

    va_end(ap);
}
