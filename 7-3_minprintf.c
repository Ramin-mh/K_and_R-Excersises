#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void minprintf(char *fmt, ...);

int main(void)
{
    char *s = "John";
    char c = 'A';
    double n = 3.55;
    minprintf("Name:%5.4s, Age: %3.1f\nchar:%3c\n", s, n, c);
}

void print_spaces(int n)
{
    for (; n > 0; n--)
        putchar(' ');
}

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...)
{
    va_list ap; /* points to each unnamed arg in turn */
    char *p, *sval;

    int specifier;
    int pad;
    int precicion;
    int len;
    int i;

    int ival;
    unsigned int uival;
    double dval;

    va_start(ap, fmt); /* make ap point to 1st unnamed arg */

    for (p = fmt; *p; p++) {
        pad = 0;
        precicion = -1;
        
        if (*p != '%') {
            putchar(*p);
            continue;
        }

        specifier = *++p;

        if (isdigit(specifier) || specifier == '-')
        {
            pad = atoi(p);
            p++;

            while (isdigit(*p))
                p++;

            specifier = *p;
        }

        if (specifier == '.')
        {
            p++;
            precicion = atoi(p);

            while (isdigit(*p))
                p++;

            specifier = *p;
        }

        switch (specifier)
        {
            case 'd':
            case 'i':
                ival = va_arg(ap, int);
                len = snprintf(NULL, 0, "%d", ival);

                while (len < precicion)
                {
                    putchar('0');
                    len++;
                }

                if (pad > len)
                {
                    print_spaces(pad - len);
                }

                printf("%d", ival);

                if (-pad > len)
                {
                    print_spaces((-pad) - len);
                }

                break;
            case 'u':
                uival = va_arg(ap, unsigned int);
                len = snprintf(NULL, 0, "%u", uival);

                while (len < precicion)
                {
                    putchar('0');
                    len++;
                }

                if (pad > len)
                {
                    print_spaces(pad - len);
                }

                printf("%u", uival);

                if (-pad > len)
                {
                    print_spaces((-pad) - len);
                }

                break;
            case 'f':
                dval = va_arg(ap, double);

                if (precicion > 0) {
                   for (i = precicion; i > 0; i--)
                   {
                       dval *= 10;
                   }
                   dval = round(dval);
                   for (i = precicion; i > 0; i--)
                   {
                       dval /= 10;
                   }
                }
                else if (precicion == 0)
                {
                    dval = round(dval);
                }
                
                len = snprintf(NULL, 0, "%f", dval);

                if (pad > len)
                {
                    print_spaces(pad - len);
                }

                printf("%f", dval);

                if (-pad > len)
                {
                    print_spaces((-pad) - len);
                }

                break;
            case 'c':
                ival = va_arg(ap, int);
                len = snprintf(NULL, 0, "%c", ival);

                if (pad > len)
                {
                    print_spaces(pad - len);
                }

                printf("%c", ival);
                
                if (-pad > len)
                {
                    print_spaces((-pad) - len);
                }

                break;
            case 's':
                sval = va_arg(ap, char *);
                len = snprintf(NULL, 0, "%s", sval);

                if (precicion >= 0 && len > precicion)
                    len = precicion;

                if (pad > len)
                {
                    print_spaces(pad - len);
                }

                if (precicion >= 0)
                {
                    for (; *sval && precicion; sval++, precicion--)
                    {
                        putchar(*sval);
                    }
                }
                else {
                    for (; *sval; sval++)
                        putchar(*sval);
                }

                if (-pad > len)
                {
                    print_spaces((-pad) - len);
                }

                break;
            default:
                putchar(*p);
                break;
        }
    }

    va_end(ap); /* clean up when done */
}
