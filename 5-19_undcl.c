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

/* undcl: convert word descriptions to declarations */
int main() {
    int type;
    char temp[MAXTOKEN];
    int last_pointer = 0;
       
    while (gettoken() != EOF) {
        strcpy(out, token);
        while ((type = gettoken()) != '\n') {
            if (type == PARENS || type == BRACKETS)
                if (last_pointer){
                    sprintf(temp,"(%s)%s", out, token);
                    strcpy(out, temp);
                    last_pointer = 0;
                }
                else {
                    strcat(out, token);
                }
            else if (type == '*') {
                sprintf(temp, "*%s", out);
                strcpy(out, temp);
                last_pointer = 1;
            }
            else if (type == NAME) {
                sprintf(temp, "%s %s", token, out);
                strcpy(out, temp);
            }
            else
                printf("invalid input at %s\n", token);
        }
        printf("%s\n", out);
    }
    return 0;
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
