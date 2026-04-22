#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

int buf = EOF;
int getch(void);
void ungetch(int);

int is_type_keyword(char *);

void dcl(char *, char *, char *);
void dirdcl(char *, char *, char *);
int gettoken(void);
int tokentype;

int error = 0;

 /* type of last token */
char token[MAXTOKEN];

 /* last token string */
char name[MAXTOKEN];
char name_arg[MAXTOKEN];

 /* identifier name */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */
char datatype_arg[MAXTOKEN];

char out[1000];
char out_arg[1000];
char tmp[1000];
int main() /* convert declaration to words */
{
    int c;
    
    while (gettoken() != EOF) {
        name[0] = '\0';
        datatype[0] = '\0';
        /* 1st token on line */
        strcpy(datatype, token); /* is the datatype */

        out[0] = '\0';

        dcl(name, datatype, out);

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
void dcl(char *name, char *datatype, char *out)
{
    int ns;

    if (error) return;

    for (ns = 0; gettoken() == '*'; ) /* count *'s */
        ns++;
    dirdcl(name, datatype, out);
    while (ns-- > 0)
        strcat(out, " pointer to");
}

/* dirdcl: parse a direct declarator */
void dirdcl(char *name, char *datatype, char *out)
{
    int type;

    if (error) return;
    
    if (tokentype == '(') {
         /* ( dcl ) */
        dcl(name, datatype, out);
        if (tokentype != ')') {
            printf("error: missing )\n");
            error = 1;
            return;
        }
    } else if (tokentype == NAME) /* variable name */
        strcpy(name, token);
    else if (tokentype == ')') {
        return;
    }
    else {
        printf("error: expected name or (dcl)\n");
        error = 1;
        return;
    }

    while ((type=gettoken()) == '(' || type == BRACKETS){
        char name_arg[MAXTOKEN];
        char datatype_arg[MAXTOKEN];
        char out_arg[1000];
        char tmp[1000];

        if (type == '('){
            strcat(out, " function(");
            do {
                out_arg[0] = '\0';
                name_arg[0] = '\0';
                gettoken();
                if (tokentype == ')'){
                    break;
                }
                strcpy(datatype_arg, token);
                while (gettoken() == NAME){
                    if (is_type_keyword(token)){
                        strcat(datatype_arg, " ");
                        strcat(datatype_arg, token);
                    }
                    else {
                        strcpy(name_arg, token);
                    }
                }
                ungetch(tokentype);
                dcl(name_arg, datatype_arg, out_arg);
                sprintf(tmp, "%s%s: %s %s, ", out, name_arg, out_arg, datatype_arg);
                strcpy(out, tmp);
            } while (tokentype == ',');
            if (tokentype != ')'){
                printf("error: expected )\n");
            }
            strcat(out, ") returning");
        }
        else
        {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
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
    if (c == '[')
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

int is_type_keyword(char *s) {
    return strcmp(s, "int") == 0 ||
           strcmp(s, "char") == 0 ||
           strcmp(s, "void") == 0 ||
           strcmp(s, "long") == 0 ||
           strcmp(s, "short") == 0 ||
           strcmp(s, "signed") == 0 ||
           strcmp(s, "unsigned") == 0 ||
           strcmp(s, "float") == 0 ||
           strcmp(s, "double") == 0 ||
           strcmp(s, "const") == 0 ||
           strcmp(s, "volatile") == 0;
}
