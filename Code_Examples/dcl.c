#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

int buf = EOF;
int getch(void);
void ungetch(int);

void dcl(void);
void dirdcl(void);
int gettoken(void);
int tokentype;

int error = 0;

 /* type of last token */
char token[MAXTOKEN];

 /* last token string */
char name[MAXTOKEN];

 /* identifier name */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */

char out[1000];
int main() /* convert declaration to words */
{
    int c;
    
    while (gettoken() != EOF) {
         /* 1st token on line */
        strcpy(datatype, token); /* is the datatype */

        out[0] = '\0';

        dcl();

        if (!error) {
            /* parse rest of line */
            if (tokentype != '\n' && tokentype != EOF){
                printf("syntax error\n");
                
            }

            printf("%s: %s %s\n", name, out, datatype);
        }
        else {
            error = 0;
        }

        if (tokentype != '\n' && tokentype != EOF){
            while ((c=getch()) != '\n' && c != EOF) { }
            if (c == EOF) break;
        }
        else if (tokentype == EOF) break;
    }
    return 0;
}

/* dcl: parse a declarator */
void dcl(void)
{
    int ns;

    if (error) return;

    for (ns = 0; gettoken() == '*'; ) /* count *'s */
        ns++;
    dirdcl();
    while (ns-- > 0)
        strcat(out, " pointer to");
}

/* dirdcl: parse a direct declarator */
void dirdcl(void)
{
    int type;

    if (error) return;
    
    if (tokentype == '(') {
         /* ( dcl ) */
        dcl();
        if (tokentype != ')') {
            printf("error: missing )\n");
            error = 1;
            return;
        }
    } else if (tokentype == NAME) /* variable name */
        strcpy(name, token);
    else {
        printf("error: expected name or (dcl)\n");
        error = 1;
        return;
    }

    while ((type=gettoken()) == PARENS || type == BRACKETS)
        if (type == PARENS)
            strcat(out, " function returning");
        else
        {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
}


int getch(void){
    int tmp;
    
    if (buf == EOF){
        return getchar();   
    }
    else {
        tmp = buf;
        buf = EOF;
        return tmp;    
    }
}

void ungetch(int c){
    if (buf == EOF){
        buf = c;
    }
    else {
        fprintf(stderr, "Buffer Overflow!, exiting...\n");
        exit(1);
    }
}

int gettoken(void) /* return next token */
{
    int c;
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t') { };
    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        }
        else {
            ungetch(c);
            return tokentype = '(';
        }
    }
    else if (c == '[')
    {
        for (*p++ = c; (*p++ = getch()) != ']'; );
        *p = '\0';
        return tokentype = BRACKETS;
    }
    else if (isalpha(c))
    {
        for (*p++ = c; isalnum(c = getch()); )
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    }
    else
        return tokentype = c;
}
